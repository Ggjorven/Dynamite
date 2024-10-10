#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#include "Dynamite/Compiler/CompilerSuite.hpp"

#include <Pulse/Types/TypeUtils.hpp>

namespace Dynamite
{

	#define PeekCheck(offset, type) (Peek(offset).has_value() && Peek(offset).value().Type == type)

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Parser::Parser(std::vector<Token>& tokens)
		: m_Tokens(tokens)
	{
	}

	Node::Program Parser::GetProgram()
	{
		Node::Program program = {};

		// Parse statements
		while (Peek().has_value())
		{
			if (auto statement = ParseStatement())
				program.Statements.emplace_back(statement.value());
			else // Failed to retrieve a valid statement
				CompilerSuite::Error(GetLineNumber(), "Failed to retrieve a valid statement");
		}

		m_Index = 0;
		return program;
	}

	/////////////////////////////////////////////////////////////////
	// Parsing functions
	/////////////////////////////////////////////////////////////////
	std::optional<Node::Reference<Node::TermExpr>> Parser::ParseTermExpr()
	{
		if (auto literalTerm = TryConsumeLiteral())
		{
			return Node::TermExpr::New(Node::LiteralTerm::New(static_cast<Node::LiteralTerm::Type>(literalTerm.value().Type), literalTerm.value()));
		}
		else if (auto identifier = TryConsume(TokenType::Identifier))
		{
			return Node::TermExpr::New(Node::IdentifierTerm::New(identifier.value()));
		}
		else if (auto parenthesis = TryConsume(TokenType::OpenParenthesis))
		{
			auto expr = ParseExpr();
			if (!expr.has_value())
			{
				CompilerSuite::Error(GetLineNumber(), "Failed to retrieve a valid expression");
				return {};
			}

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`");

			return Node::TermExpr::New(Node::ParenthesisTerm::New(expr.value()));
		}
			
		return {};
	}

	std::optional<Node::Reference<Node::Expression>> Parser::ParseExpr(const size_t minimumPrecedence)
	{
		if (auto termLHS = ParseTermExpr())
		{
			/////////////////////////////////////////////////////////////////
			// Type retrieval
			/////////////////////////////////////////////////////////////////
			ValueType type = std::visit([this](auto&& obj) -> ValueType
			{
				if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Node::Reference<Node::LiteralTerm>>)
					return GetValueType(obj->TokenObj.Type, obj->TokenObj.Value.value());
				else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Node::Reference<Node::IdentifierTerm>>)
					return GetVar(obj->TokenObj.Value.value()).Type;
				else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Node::Reference<Node::ParenthesisTerm>>)
					return obj->ExprObj->Type;

				DY_LOG_ERROR("Retrieving ValueType has not been implemented for current type.");
				return {};
			}, termLHS.value()->TermObj);

			/////////////////////////////////////////////////////////////////
			// Expression retrieval/creation
			/////////////////////////////////////////////////////////////////
			Node::Reference<Node::Expression> exprLHS = Node::Expression::New(type, termLHS.value());
			
			while (true)
			{
				std::optional<Token> current = Peek(0);
				std::optional<size_t> precedence = {};
				size_t nextMinimumPrecedence = -1;
				
				// Note: It breaks out and just returns the normal expression if
				// it's not a binary expression.
				if (!current.has_value())
					break;

				precedence = Node::GetBinaryExprPrecendce(static_cast<Node::BinaryExpr::Type>(current->Type));
				if (!precedence.has_value() || precedence.value() < minimumPrecedence)
					break;
				else
					nextMinimumPrecedence = precedence.value() + 1;

				Token operation = Consume();

				auto exprRHS = ParseExpr(nextMinimumPrecedence);
				if (!exprRHS.has_value()) 
				{
					CompilerSuite::Error(GetLineNumber(), "Unable to parse expression.");
					break;
				}

				auto expr = Node::BinaryExpr::New(static_cast<Node::BinaryExpr::Type>(operation.Type));
				auto exprLHS2 = Node::Expression::New(type, exprLHS->ExprObj);
				
				expr->LHS = exprLHS2;
				expr->RHS = exprRHS.value();

				exprLHS->ExprObj = expr;
			}

			// Note: This is either a binary expression or just a normal expression.
			// The loop up top accounts for both
			return exprLHS;
		}

		return {};
	}

	std::optional<Node::Reference<Node::ScopeStatement>> Parser::ParseScope()
	{
		if (!TryConsume(TokenType::OpenCurlyBrace).has_value())
			return {};

		m_Scopes.push_back(m_Variables.size());

		Node::Reference<Node::ScopeStatement> scope = Node::ScopeStatement::New();
		while (auto stmt = ParseStatement()) 
			scope->Statements.push_back(stmt.value());

		// Note: We decrement, since if we did not retrieve any (valid) statements
		// it will result in consuming the next token just to carry on, but this also means
		// that the '}' has already been consumed. So we just go one back.
		m_Index--;

		size_t popCount = m_Variables.size() - m_Scopes.back();
		PopVar(popCount);
		m_Scopes.pop_back();
		
		CheckConsume(TokenType::CloseCurlyBrace, "Expected `}}`");
		return scope;
	}

	std::optional<Node::Reference<Node::ConditionBranch>> Parser::ParseConditionBrach()
	{
		if (PeekCheck(0, TokenType::Else) && PeekCheck(1, TokenType::If)) 
		{
			Consume(); // 'else' token
			Consume(); // 'if' token

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpr())
			{
				auto elif = Node::ElseIfBranch::New(expr.value());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

				if (auto scope = ParseScope())
				{
					elif->Scope = scope.value();
					elif->Next = ParseConditionBrach(); // Note: Can be NULL
					return Node::ConditionBranch::New(elif);
				}
				else
					CompilerSuite::Error(GetLineNumber(), "Failed to retrieve valid scope.");
			}
			else
				CompilerSuite::Error(GetLineNumber(), "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			return {};
		}
		else if (PeekCheck(0, TokenType::Else)) 
		{
			Consume(); // 'else' token

			if (auto scope = ParseScope()) 
				return Node::ConditionBranch::New(Node::ElseBranch::New(scope.value()));
			else 
				CompilerSuite::Error(GetLineNumber(), "Failed to retrieve valid scope.");

			return {};
		}

		return {};
	}

	std::optional<Node::Reference<Node::Statement>> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Exit statement (Enforces UInt8 expr :) ) 
		// TODO: Make a table with functions and make code underneath reusable.
		/////////////////////////////////////////////////////////////////
		if (PeekCheck(0, TokenType::Exit) && PeekCheck(1, TokenType::OpenParenthesis))
		{
			Consume(); // Exit token
			Consume(); // '(' token

			Node::Reference<Node::ExitStatement> exitStatement = Node::ExitStatement::New();

			// Expression resolution
			if (auto expr = ParseExpr())
			{
				// Enforce Int32 type
				if (!ValueTypeCastable(expr.value()->Type, ValueType::UInt8))
				{
					CompilerSuite::Error(GetLineNumber(), "exit() expects an u8 type, got {0}, {0} is not castable to u8", ValueTypeToStr(expr.value()->Type));

					// Close parenthesis ')' & semicolon `;` resolution
					CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return {};
				}
				
				// Note: Only casts if the internal type is a literalterm
				CastInternalValue(expr.value()->Type, ValueType::UInt8, expr.value());
				exitStatement->ExprObj = expr.value();
			}
			else
				CompilerSuite::Error(GetLineNumber(), "Invalid expression.");

			// Close parenthesis ')' & semicolon `;` resolution
			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return Node::Statement::New(exitStatement);
		}

		/////////////////////////////////////////////////////////////////
		// Scope
		/////////////////////////////////////////////////////////////////
		else if (PeekCheck(0, TokenType::OpenCurlyBrace))
		{
			if (auto scope = ParseScope()) 
				return Node::Statement::New(scope.value());
			else
				CompilerSuite::Error(GetLineNumber(), "Invalid scope.");
		}

		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		else if (auto if_ = TryConsume(TokenType::If)) 
		{
			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpr()) 
			{
				auto ifStatement = Node::IfStatement::New(expr.value());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			
				if (auto scope = ParseScope())
				{
					ifStatement->Scope = scope.value();
					ifStatement->Next = ParseConditionBrach(); // Note: Can be NULL
					return Node::Statement::New(ifStatement);
				}
				else
					CompilerSuite::Error(GetLineNumber(), "Failed to retrieve valid scope.");
			}
			else 
				CompilerSuite::Error(GetLineNumber(), "Invalid expression.");
			
			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			return {};
		}

		/////////////////////////////////////////////////////////////////
		// Variable creation
		/////////////////////////////////////////////////////////////////
		else if (PeekIsVariableType() && PeekCheck(1, TokenType::Identifier) && PeekCheck(2, TokenType::Equals))
		{
			Token typeToken = Consume(); // Type token
			ValueType variableType = static_cast<ValueType>(typeToken.Type);

			Node::Reference<Node::VariableStatement> variable = Node::VariableStatement::New(variableType, Consume()); // Identifier token

			std::string varName = variable->TokenObj.Value.value();

			// Add type to current scope with name of variable
			PushVar(varName, variableType);

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpr())
			{
				if (!ValueTypeCastable(expr.value()->Type, variableType))
				{
					CompilerSuite::Error(GetLineNumber(), "Variable creation of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", varName, ValueTypeToStr(variableType), ValueTypeToStr(expr.value()->Type));

					// Semicolon `;` resolution
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return {};
				}

				// Note: Only casts if the internal type is a literalterm
				CastInternalValue(expr.value()->Type, variableType, expr.value());
				variable->ExprObj = expr.value();
			}
			else
				CompilerSuite::Error(GetLineNumber(), "Invalid expression.");

			// Semicolon ';' resolution
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return Node::Statement::New(variable);
		}

		// Consume() just 1 token, just to make sure we keep going.
		// Since obviously from the previous token it was impossible to carry on.
		Consume();
		return {};
	}

	/////////////////////////////////////////////////////////////////
	// Peeking & consuming
	/////////////////////////////////////////////////////////////////
	std::optional<Token> Parser::Peek(size_t offset) const
	{
		if (m_Index + offset >= m_Tokens.size())
			return {};

		return m_Tokens.at(m_Index + offset);
	}

	Token Parser::Consume()
	{
		return m_Tokens.at(m_Index++);
	}

	Token Parser::CheckConsume(TokenType tokenType, const std::string& msg)
	{
		if (PeekCheck(0, tokenType))
			return Consume();
		else if (!msg.empty())
			CompilerSuite::Error(GetLineNumber(), msg);

		return {};
	}

	std::optional<Token> Parser::TryConsume(TokenType type)
	{
		if (Peek(0).has_value() && Peek(0).value().Type == type)
			return Consume();
			
		return {};
	}

	std::optional<Token> Parser::TryConsumeLiteral()
	{
		if (PeekIs({
				TokenType::BoolLiteral,
				TokenType::IntegerLiteral,
				TokenType::FloatLiteral,
				TokenType::CharLiteral,
				TokenType::StringLiteral
			}))
		{
			return Consume();
		}

		return {};
	}

	bool Parser::PeekIs(const std::vector<TokenType>& allowedValues)
	{
		std::optional<Token> peek = Peek(0);
		if (!peek.has_value())
			return false;

		TokenType type = peek.value().Type;
		
		for (const auto& allowed : allowedValues)
		{
			if (type == allowed)
				return true;
		}

		return false;
	}

	// Note: Has to be manually updated
	bool Parser::PeekIsVariableType()
	{
		return PeekIs({
			TokenType::Bool, 

			TokenType::Int8,
			TokenType::Int16,
			TokenType::Int32,
			TokenType::Int64,

			TokenType::UInt8,
			TokenType::UInt16,
			TokenType::UInt32,
			TokenType::UInt64,

			TokenType::Float32,
			TokenType::Float64,

			TokenType::Char,
			TokenType::String
		});
	}

	// Note: Has to be manually updated
	bool Parser::PeekIsBinaryOperator()
	{
		return PeekIs({
			TokenType::Add,
			TokenType::Subtract,
			TokenType::Multiply,
			TokenType::Divide,

			TokenType::Or,
			TokenType::And,
			TokenType::Xor
		});
	}

	// Note: Only casts if the internal type is a literalterm
	void Parser::CastInternalValue(ValueType from, ValueType to, Node::Reference<Node::Expression> expression)
	{
		if (from == to)
			return;

		bool dataLost = false;
		std::string originalData = Node::FormatExpressionData(expression);
		std::visit([&, from, to](auto&& obj) -> bool
		{
			if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Node::Reference<Node::TermExpr>>)
			{
				std::visit([&, from, to](auto&& obj) -> void
				{
					if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Node::Reference<Node::LiteralTerm>>)
					{
						obj->TokenObj.Value = ValueTypeCast(from, to, obj->TokenObj.Value.value(), &dataLost);
					}

				}, obj->TermObj);
			}

			return dataLost;
		}, expression->ExprObj);

		if (dataLost) // Note: This outputs the new value, not the original value before cast
			CompilerSuite::Warn(GetLineNumber(), "Lost data while casting expression. From: {0}, to {1}\n    Original: \t\t{2}\n    New: \t\t{3}", ValueTypeToStr(from), ValueTypeToStr(to), originalData, Node::FormatExpressionData(expression));
	}

	void Parser::PushVar(const std::string& name, ValueType type)
	{
		m_Variables.emplace_back(name, type);
	}

	void Parser::PopVar(size_t count)
	{
		for (size_t i = 0; i < count; i++)
			m_Variables.pop_back();
	}

	Variable Parser::GetVar(const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Variables), [&](const Variable& var) 
		{
			return var.Name == name;
		});

		if (it == m_Variables.cend())
		{
			CompilerSuite::Error(GetLineNumber(), "Undeclared identifier: {0}", name);
			return {};
		}
	
		return *it;
	}

}
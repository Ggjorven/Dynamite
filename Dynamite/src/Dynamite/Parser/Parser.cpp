#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

namespace Dynamite
{

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
		while (Peek(0).HasValue())
		{
			if (auto variableStatement = ParseVariableStatement())
				program.Definitions.emplace_back(variableStatement.Value());
			else if (auto function = ParseFunction())
				program.Definitions.emplace_back(function.Value());
			else 
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve a valid variable or function definition.");
		}

		m_Index = 0;
		m_Scopes.Reset();
		return program;
	}

	/////////////////////////////////////////////////////////////////
	// Parsing functions
	/////////////////////////////////////////////////////////////////
	Optional<Node::Reference<Node::TermExpr>> Parser::ParseTermExpr()
	{
		/////////////////////////////////////////////////////////////////
		// Literal
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeLiterals()))
		{
			auto consumed = Consume();

			Node::Reference<Node::TermExpr> termExpr = Node::New<Node::TermExpr>();

			Type literalType = TypeSystem::GetTypeFromLiteral(consumed.Type, consumed.Value);
			Token literalToken = consumed;
			
			termExpr->Term = Node::New<Node::LiteralTerm>(literalType, literalToken);

			return termExpr;
		}

		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier))
		{
			auto consumed = Consume();

			Node::Reference<Node::TermExpr> termExpr = Node::New<Node::TermExpr>();

			Type identifierType = m_Scopes.GetVariableType(consumed.Value);
			Token identifierToken = consumed;

			termExpr->Term = Node::New<Node::IdentifierTerm>(identifierType, identifierToken);

			return termExpr;
		}

		/////////////////////////////////////////////////////////////////
		// Parenthesis
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenParenthesis))
		{
			Consume(); // '(' token

			auto expr = ParseExpression();
			if (!expr.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve a valid expression");
				return {};
			}

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`");

			return Node::New<Node::TermExpr>(Node::New<Node::ParenthesisTerm>(expr.Value()));
		}

		return {};
	}



	Optional<Node::Reference<Node::Expression>> Parser::ParseExpression(size_t minimumPrecedence)
	{
		/////////////////////////////////////////////////////////////////
		// TermExpr
		/////////////////////////////////////////////////////////////////
		if (auto term = ParseTermExpr())
		{
			/////////////////////////////////////////////////////////////////
			// Type retrieval
			/////////////////////////////////////////////////////////////////
			const Type& type = term.Value()->GetType();

			/////////////////////////////////////////////////////////////////
			// Expression retrieval/creation
			/////////////////////////////////////////////////////////////////
			Node::Reference<Node::Expression> expr = Node::New<Node::Expression>(term.Value());

			while (true)
			{
				Optional<Token> current = Peek(0);
				Optional<size_t> precedence = {};
				size_t nextMinimumPrecedence = -1;

				// Note: It breaks out and just returns the normal expression if
				// it's not a binary expression.
				if (!current.HasValue())
					break;

				precedence = Node::GetBinaryPrecendence(current.Value().Type);
				if (!precedence.HasValue() || precedence.Value() < minimumPrecedence)
					break;
				else
					nextMinimumPrecedence = precedence.Value() + 1;

				auto exprLHS = Node::New<Node::Expression>(expr->Expr);
				Token operation = Consume();

				auto exprRHS = ParseExpression(nextMinimumPrecedence);
				if (!exprRHS.HasValue())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Unable to parse expression.");
					break;
				}

				auto binExpr = Node::New<Node::BinaryExpr>(operation.Type, TypeSystem::GetResultType(exprLHS->GetType(), exprRHS.Value()->GetType()), exprLHS, exprRHS.Value());

				// Set the result expr to the computed binary expression
				expr->Expr = binExpr;
			}

			// Note: This is either a binary expression or just a normal expression.
			// The loop up top accounts for both
			return expr;
		}

		return {};
	}



	Optional<Node::Reference<Node::ConditionBranch>> Parser::ParseConditionBrach()
	{
		/////////////////////////////////////////////////////////////////
		// Else If Branch
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Else) 
			&& Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::If))
		{
			Consume(); // 'else' token
			Consume(); // 'if' token

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				auto elif = Node::New<Node::ElseIfBranch>(expr.Value());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

				if (auto scope = ParseScopeStatement())
				{
					elif->Scope = scope.Value();
					elif->Next = ParseConditionBrach(); // Note: Can be NullRef

					return Node::New<Node::ConditionBranch>(elif);
				}
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			return {};
		}

		/////////////////////////////////////////////////////////////////
		// Else Branch
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Else))
		{
			Consume(); // 'else' token

			if (auto scope = ParseScopeStatement())
			{
				return Node::New<Node::ConditionBranch>(Node::New<Node::ElseBranch>(scope.Value()));
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");

			return {};
		}

		return {};
	}



	Optional<Node::Reference<Node::IfStatement>> Parser::ParseIfStatement()
	{
		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::If))
		{
			auto consumed = Consume();

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				auto ifStatement = Node::New<Node::IfStatement>(expr.Value());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

				if (auto scope = ParseScopeStatement())
				{
					ifStatement->Scope = scope.Value();
					ifStatement->Next = ParseConditionBrach(); // Note: Can be NullRef

					return ifStatement;
				}
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			return {};
		}

		return {};
	}



	Optional<Node::Reference<Node::ScopeStatement>> Parser::ParseScopeStatement(bool startScope)
	{
		if (!Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
			return {};

		Consume(); // '{' token

		if (startScope)
			m_Scopes.BeginScope();

		Node::Reference<Node::ScopeStatement> scope = Node::New<Node::ScopeStatement>();
		while (auto statement = ParseStatement())
			scope->Statements.push_back(statement.Value());

		m_Scopes.EndScope();

		CheckConsume(TokenType::CloseCurlyBrace, "Expected `}}`");
		return scope;
	}



	Optional<Node::Reference<Node::VariableStatement>> Parser::ParseVariableStatement()
	{
		size_t offset = 0;
		
		/////////////////////////////////////////////////////////////////
		// Variable initiazation
		/////////////////////////////////////////////////////////////////
		if (PeekIsType(offset) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Equals))
		{
			Type type = GetType();

			Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>(type, Consume()); // Identifier token

			std::string varName = variable->Variable.Value;
			m_Scopes.PushVar(varName, type);

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpression())
			{
				if (!TypeSystem::Castable(expr.Value()->GetType(), type))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Variable creation of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", varName, TypeSystem::ToString(type), TypeSystem::ToString(expr.Value()->GetType()));

					// Semicolon `;` resolution
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return variable;
				}

				// Note: Only casts if the internal type is a literalterm
				// TODO: Add back casting
				//TypeSystem::Cast(expr.Value()->GetType(), type, expr.Value());
				variable->Expr = expr.Value();
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			// Semicolon ';' resolution
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return variable;
		}
		
		/////////////////////////////////////////////////////////////////
		// Variable Default Initialization
		/////////////////////////////////////////////////////////////////
		offset = 0;

		if (PeekIsType(offset) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Semicolon))
		{
			Type type = GetType();

			Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>(type, Consume()); // Identifier token

			std::string varName = variable->Variable.Value;
			m_Scopes.PushVar(varName, type);

			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return variable;
		}

		return {};
	}



	Optional<Node::Reference<Node::Statement>> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Scope
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
		{
			if (auto scope = ParseScopeStatement())
				return Node::New<Node::Statement>(scope.Value());
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid scope.");
		}

		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		else if (auto if_ = ParseIfStatement())
		{
			return Node::New<Node::Statement>(if_.Value());
		}

		/////////////////////////////////////////////////////////////////
		// Variable creation
		/////////////////////////////////////////////////////////////////
		else if (auto var = ParseVariableStatement())
		{
			return Node::New<Node::Statement>(var.Value());
		}

		/////////////////////////////////////////////////////////////////
		// Variable assignment
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier) && 
			Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Equals))
		{
			auto assignment = Node::New<Node::AssignmentStatement>(Consume());
			Consume(); // '=' char

			if (auto expr = ParseExpression())
			{
				assignment->Expr = expr.Value();
				CheckConsume(TokenType::Semicolon, "Expected `;`.");

				return Node::New<Node::Statement>(assignment);
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			return {};
		}

		return {};
	}



	Optional<Node::Reference<Node::Function>> Parser::ParseFunction()
	{
		size_t offset = 0;
		
		/////////////////////////////////////////////////////////////////
		// Function
		/////////////////////////////////////////////////////////////////
		if (PeekIsType(offset) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::OpenParenthesis))
		{
			Type returnType = GetType();

			Node::Reference<Node::Function> func = Node::New<Node::Function>(returnType, Consume()); // Identifier token

			std::string funcName = func->Name.Value;

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			while (true)
			{
				size_t offset = 0;

				if (PeekIsType(offset) && 
					Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier))
				{
					Type variableType = GetType();

					Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>(variableType, Consume()); // Identifier token

					std::string varName = variable->Variable.Value;

					// Note: We begin the scope here so we can 
					// expose the parameters to the scope as well.
					m_Scopes.BeginScope();

					m_Scopes.PushVar(varName, variableType);

					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Equals))
					{
						Consume(); // '=' token

						if (auto expr = ParseExpression())
							variable->Expr = expr.Value();
						else
							Compiler::Error("Expected expression.");
					}

					func->Parameters.push_back(variable);

					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Comma))
					{
						Consume(); // ',' token
						continue;
					}
				}
				else
					break;
			}

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			if (auto body = ParseScopeStatement(false))
				func->Body = body.Value();
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Expected scope.");

			return func;
		}

		return {};
	}

	/////////////////////////////////////////////////////////////////
	// Peeking & consuming
	/////////////////////////////////////////////////////////////////
	Optional<Token> Parser::Peek(size_t offset) const
	{
		if (m_Index + offset >= m_Tokens.size())
			return {};

		return m_Tokens.at(m_Index + offset);
	}

	Token Parser::Consume()
	{
		return m_Tokens.at(m_Index++);
	}

	void Parser::CheckConsume(TokenType expect, const std::string& msg)
	{
		if (Utils::OptMemberIs(Peek(0), &Token::Type, expect))
			Consume();
		else if (!msg.empty())
			Compiler::Error(Peek(0).Value().LineNumber, msg);
	}

	/////////////////////////////////////////////////////////////////
	// Other
	/////////////////////////////////////////////////////////////////
	bool Parser::PeekIsType(size_t& offset)
	{
		size_t startOffset = offset;

		Type result = {};

		// Front Qualifiers
		while (Utils::OptMemberIs(Peek(offset), &Token::Type, GetAllTokenTypeQualifiers()))
			offset++;

		// Check if it's a Type
		if (Utils::OptMemberIs(Peek(offset), &Token::Type, GetAllTokenTypeTypes()))
			offset++;
		else
		{
			offset = startOffset;
			return false;
		}

		// Back Qualifiers
		while (Utils::OptMemberIs(Peek(offset), &Token::Type, GetAllTokenTypeQualifiers()))
			offset++;

		return true;
	}

	Type Parser::GetType()
	{
		Type result = {};

		// Front Qualifiers
		while (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeQualifiers()))
			result.FrontQualifiers.push_back(TokenTypeToTypeQualifier(Consume().Type));

		// Type
		{
			result.Information.Specifier = TokenTypeToTypeSpecifier(Consume().Type);

			if (result.Information.Specifier == TypeSpecifier::Identifier)
				result.Information.Value = Peek(-1).Value().Value;
		}

		// Back Qualifiers
		while (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeQualifiers()))
			result.BackQualifiers.push_back(TokenTypeToTypeQualifier(Consume().Type));

		return result;
	}

}
#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#include <Pulse/Types/TypeUtils.hpp>

namespace Dynamite
{

	#define PeekCheck(offset, type) (Peek(offset).has_value() && Peek(offset).value().Type == type)

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Parser::Parser(const std::vector<Token>& tokens)
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
				DY_LOG_ERROR("Failed to retrieve a valid statement.");
		}

		m_Index = 0;
		return program;
	}

	void Parser::Print(const Node::Program& program)
	{
		for (const auto& statement : program.Statements)
			DY_LOG_TRACE("{0}",  Node::FormatStatementData(statement));
	}

	/////////////////////////////////////////////////////////////////
	// Parsing functions
	/////////////////////////////////////////////////////////////////
	std::optional<Node::Reference<Node::TermExpr>> Parser::ParseTermExpr()
	{
		if (auto literalTerm = TryConsumeLiteral())
			return Node::TermExpr::New(Node::LiteralTerm::New(static_cast<Node::LiteralTerm::Type>(literalTerm.value().Type), literalTerm.value()));
		else if (auto identifier = TryConsume(TokenType::Identifier))
			return Node::TermExpr::New(Node::IdentifierTerm::New(identifier.value()));
			
		return {};
	}

	std::optional<Node::Reference<Node::Expression>> Parser::ParseExpr()
	{
		if (auto term = ParseTermExpr()) 
		{
			ValueType type = ValueType::None;
			Token termToken = term.value()->GetToken();
			
			// On identifier retrieve type from symbols
			if (termToken.Type == TokenType::Identifier)
				type = m_SymbolTypes[termToken.Value.value()];
			else // It's a literal
				type = GetValueType(termToken.Type, termToken.Value.value());
			
			// Check if Term is Binary expression
			if (PeekIsBinaryOperator()) // Note: Takes type of LHS
			{
				Token op = Consume(); // Binary operator

				auto binaryExpr = Node::BinaryExpr::New(static_cast<Node::BinaryExpr::Type>(op.Type));

				auto lhs = Node::Expression::New(type, term.value());
				binaryExpr->LHS = lhs;

				if (auto rhs = ParseExpr()) 
				{
					binaryExpr->RHS = rhs.value();
					return Node::Expression::New(type, binaryExpr);
				}

				DY_LOG_ERROR("Expected expression.");
				return {};
			}
			else // Else it's just a literal 
			{
				auto expr = Node::Expression::New(type, term.value());
				return expr;
			}
		}
		
		return {};
	}

	std::optional<Node::Reference<Node::Statement>> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Exit statement (Enforces Int32 expr :) ) 
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
				if (!ValueTypeCastable(expr.value()->Type, ValueType::Int32))
				{
					DY_LOG_ERROR("exit() expects an Int32 type, got {0}, {0} is not castable to Int32", ValueTypeToStr(expr.value()->Type));

					// Close parenthesis ')' & semicolon `;` resolution
					CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return {};
				}
				
				// Note: Only casts if the internal type is a literalterm
				CastInternalValue(expr.value()->Type, ValueType::Int32, expr.value());
				exitStatement->ExprObj = expr.value();
			}
			else
				DY_LOG_ERROR("Invalid expression.");

			// Close parenthesis ')' & semicolon `;` resolution
			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return Node::Statement::New(exitStatement);
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

			// Add type to global scope with name of variable
			m_SymbolTypes[varName] = variableType;

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpr())
			{
				if (!ValueTypeCastable(expr.value()->Type, variableType))
				{
					DY_LOG_ERROR("Variable creation of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}", varName, ValueTypeToStr(variableType), ValueTypeToStr(expr.value()->Type));

					// Close parenthesis ')' & semicolon `;` resolution
					CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return {};
				}

				// Note: Only casts if the internal type is a literalterm
				CastInternalValue(expr.value()->Type, variableType, expr.value());
				variable->ExprObj = expr.value();
			}
			else
				DY_LOG_ERROR("Invalid expression.");

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
			DY_LOG_ERROR(msg);

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
		std::optional<Token> peek = Peek();
		if (peek.has_value())
		{
			TokenType type = peek.value().Type;

			if (type == TokenType::BoolLiteral || 
				type == TokenType::IntegerLiteral ||
				type == TokenType::FloatLiteral ||
				type == TokenType::CharLiteral ||
				type == TokenType::StringLiteral
				)
			{
				return Consume();
			}
		}

		return {};
	}

	// Note: Has to be manually updated
	bool Parser::PeekIsVariableType()
	{
		if (!Peek(0).has_value())
			return false;

		#define CheckType(type) peeked.value().Type == type
		auto peeked = Peek(0);

		bool result = false;

		result |= CheckType(TokenType::Bool);
		
		result |= CheckType(TokenType::Int8);
		result |= CheckType(TokenType::Int16);
		result |= CheckType(TokenType::Int32);
		result |= CheckType(TokenType::Int64);

		result |= CheckType(TokenType::UInt8);
		result |= CheckType(TokenType::UInt16);
		result |= CheckType(TokenType::UInt32);
		result |= CheckType(TokenType::UInt64);

		result |= CheckType(TokenType::Float32);
		result |= CheckType(TokenType::Float64);

		result |= CheckType(TokenType::Char);
		result |= CheckType(TokenType::String);
		
		return result;
	}

	bool Parser::PeekIsBinaryOperator()
	{
		if (!Peek(0).has_value())
			return false;

		#define CheckOperator(op) peeked.value().Type == op
		auto peeked = Peek(0);

		bool result = false;
		result |= CheckOperator(TokenType::Plus);
		result |= CheckOperator(TokenType::Minus);
		result |= CheckOperator(TokenType::Star);
		result |= CheckOperator(TokenType::Divide);

		result |= CheckOperator(TokenType::Or);
		result |= CheckOperator(TokenType::And);
		result |= CheckOperator(TokenType::Xor);

		return result;
	}

	ValueType Parser::GetValueType(TokenType literalType, const std::string& value)
	{
		ValueType type = ValueType::None;

		switch (literalType)
		{
		case TokenType::BoolLiteral:
		{
			type = ValueType::Bool;
			break;
		}
		case TokenType::IntegerLiteral:
		{
			bool isNegative = !value.empty() && value[0] == '-';

			if (isNegative)
			{
				int64_t intVal = std::stoll(value);
				
				if (intVal >= Pulse::Numeric::Min<int8_t>() && intVal <= Pulse::Numeric::Max<int8_t>())
					type = ValueType::Int8;
				else if (intVal >= Pulse::Numeric::Min<int16_t>() && intVal <= Pulse::Numeric::Max<int16_t>())
					type = ValueType::Int16;
				else if (intVal >= Pulse::Numeric::Min<int32_t>() && intVal <= Pulse::Numeric::Max<int32_t>())
					type = ValueType::Int32;
				else if (intVal >= Pulse::Numeric::Min<int64_t>() && intVal <= Pulse::Numeric::Max<int64_t>())
					type = ValueType::Int64;
				else
					DY_LOG_ERROR("Integer {0} exceeds max integers' type (Int64) size.", value);
			}
			else
			{
				uint64_t uintVal = std::stoull(value);

				if (uintVal <= Pulse::Numeric::Max<uint8_t>())
					type = ValueType::UInt8;
				else if (uintVal <= Pulse::Numeric::Max<uint16_t>())
					type = ValueType::UInt16;
				else if (uintVal <= Pulse::Numeric::Max<uint32_t>())
					type = ValueType::UInt32;
				else if (uintVal <= Pulse::Numeric::Max<uint64_t>())
					type = ValueType::UInt64;
				else
					DY_LOG_ERROR("Integer {0} exceeds max integers' type (UInt64) size.", value);
			}
			break;
		}
		case TokenType::FloatLiteral:
		{
			double doubleVal = std::stod(value);

			if (doubleVal >= Pulse::Numeric::Min<float>() && doubleVal <= Pulse::Numeric::Max<float>())
				type = ValueType::Float32;
			if (doubleVal >= Pulse::Numeric::Min<double>() && doubleVal <= Pulse::Numeric::Max<double>())
				type = ValueType::Float32;
			else
				DY_LOG_ERROR("Float {0} exceeds max floats' type (Float64) size.", value);

			break;
		}
		case TokenType::CharLiteral:
		{
			type = ValueType::Char;
			break;
		}
		case TokenType::StringLiteral:
		{
			type = ValueType::String;
			break;
		}

		default:
			break;
		}

		return type;
	}

	// Note: Only casts if the internal type is a literalterm
	void Parser::CastInternalValue(ValueType from, ValueType to, Node::Reference<Node::Expression> expression)
	{
		if (from == to)
			return;

		struct ExprVisitor
		{
		public:
			ValueType From;
			ValueType To;
			bool DataLost = false;

		public:
			void operator() (Node::Reference<Node::TermExpr> obj)
			{
				struct TermVisitor
				{
				public:
					ValueType From;
					ValueType To;
					bool* DataLost;

				public:
					void operator() (Node::Reference<Node::LiteralTerm> obj)
					{
						obj->TokenObj.Value = ValueTypeCast(From, To, obj->TokenObj.Value.value(), DataLost);
					}
					void operator() (Node::Reference<Node::IdentifierTerm> obj) {}
				};

				TermVisitor visitor = { .From = From, .To = To, .DataLost = &DataLost };
				std::visit(visitor, obj->TermObj);
			}
			void operator() (Node::Reference<Node::BinaryExpr> obj) {}
		};

		ExprVisitor visitor = { .From = from, .To = to };
		std::visit(visitor, expression->ExprObj);

		if (visitor.DataLost) // Note: This outputs the new value, not the original value before cast
			DY_LOG_WARN("Lost data while casting expression {0}. From: {1}, to {2}", Node::FormatExpressionData(expression), ValueTypeToStr(from), ValueTypeToStr(to));
	}

}
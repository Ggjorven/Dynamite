#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

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
				if (expr.value()->Type != ValueType::Int32)
				{
					DY_LOG_ERROR("exit() expects an Int32 type, got {0}", Pulse::Enum::Name(expr.value()->Type));
					return {};
				}

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

			// Add type to global scope with name of variable
			m_SymbolTypes[variable->TokenObj.Value.value()] = variableType;

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpr())
				variable->ExprObj = expr.value();
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

			if (/*type == TokenType::BoolLiteral || */
				type == TokenType::IntegerLiteral /*||
				type == TokenType::FloatLiteral ||
				type == TokenType::CharLiteral ||
				type == TokenType::StringLiteral
				*/)
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
		result |= CheckType(TokenType::Int8);
		result |= CheckType(TokenType::Int16);
		result |= CheckType(TokenType::Int32);
		result |= CheckType(TokenType::Int64);

		result |= CheckType(TokenType::UInt8);
		result |= CheckType(TokenType::UInt16);
		result |= CheckType(TokenType::UInt32);
		result |= CheckType(TokenType::UInt64);
		
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
		// TODO: More operators

		return result;
	}

	ValueType Parser::GetValueType(TokenType literalType, const std::string& value)
	{
		ValueType type = ValueType::None;

		switch (literalType)
		{
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
			}
			break;
		}

		default:
			break;
		}

		return type;
	}

}
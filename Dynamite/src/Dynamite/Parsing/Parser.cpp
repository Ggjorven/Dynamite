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
		if (auto integerLiteral = TryConsume(TokenType::IntegerLiteral))
			return Node::TermExpr::New(Node::IntegerLiteralTerm::New(integerLiteral.value()));
		else if (auto identifier = TryConsume(TokenType::Identifier))
			return Node::TermExpr::New(Node::IdentifierTerm::New(identifier.value()));
			
		return {};
	}

	std::optional<Node::Reference<Node::Expression>> Parser::ParseExpr()
	{
		if (auto term = ParseTermExpr()) 
		{
			ValueType type = static_cast<ValueType>(term.value()->GetToken().Type);
			
			// Check if Term is Binary expression
			if (PeekIsBinaryOperator()) // Note: Takes type of LHS
			{
				Token op = Consume(); // Binary operator

				auto binaryExpr = Node::BinaryExpr::New(static_cast<Node::BinaryExpr::Type>(op.Type));

				auto lhs = Node::Expression::New(term.value(), type);
				binaryExpr->LHS = lhs;

				if (auto rhs = ParseExpr()) 
				{
					binaryExpr->RHS = rhs.value();
					return Node::Expression::New(binaryExpr, type);
				}

				DY_LOG_ERROR("Expected expression.");
				return {};
			}
			else // Else it's just a literal 
			{
				auto expr = Node::Expression::New(term.value(), type);
				return expr;
			}
		}
		
		return {};
	}

	std::optional<Node::Reference<Node::Statement>> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Exit statement
		/////////////////////////////////////////////////////////////////
		if (PeekCheck(0, TokenType::Exit) && PeekCheck(1, TokenType::OpenParenthesis))
		{
			Consume(); // Exit token
			Consume(); // '(' token

			Node::Reference<Node::ExitStatement> exitStatement = Node::ExitStatement::New();

			// Expression resolution
			if (auto expr = ParseExpr())
				exitStatement->ExprObj = expr.value();
			else
				DY_LOG_ERROR("Invalid expression.");

			// Close parenthesis ')' & semicolon `;` resolution
			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return Node::Statement::New(exitStatement);
		}

		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (PeekIsVariableType() && PeekCheck(1, TokenType::Identifier) && PeekCheck(2, TokenType::Equals))
		{
			Token variableType = Consume(); // Type token
			Node::Reference<Node::VariableStatement> variable = Node::VariableStatement::New(Consume()); // Identifier token

			// Add type to global scope with name of variable
			m_SymbolTypes[variable->TokenObj.Value.value()] = static_cast<ValueType>(variableType.Type);

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

	// Note: Has to be manually updated
	bool Parser::PeekIsVariableType()
	{
		if (!Peek(0).has_value())
			return false;

		#define CheckType(type) Peek(0).value().Type == type

		bool result = false;
		//result |= CheckType(TokenType::Int8);
		//result |= CheckType(TokenType::Int16);
		//result |= CheckType(TokenType::Int32);
		result |= CheckType(TokenType::Int64);
		
		return result;
	}

	bool Parser::PeekIsBinaryOperator()
	{
		if (!Peek(0).has_value())
			return false;

		#define CheckOperator(op) Peek(0).value().Type == op

		bool result = false;
		result |= CheckOperator(TokenType::Plus);
		// TODO: More operators

		return result;
	}

}
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

	Nodes::Program Parser::GetProgram()
	{
		Nodes::Program program = {};

		// Parse statements
		while (Peek().has_value()) 
		{
			if (auto statement = ParseStatement())
				program.Statements.emplace_back(statement.value());

			// Failed to retrieve a valid statement
			else
				DY_LOG_ERROR("Failed to retrieve a valid statement.");
		}

		m_Index = 0;
		return program;
	}

	void Parser::Print(const Nodes::Program& program)
	{
		for (const auto& statement : program.Statements)
			DY_LOG_TRACE("[{0}] - {1}", Pulse::Enum::Name(statement->StatementType), Nodes::FormatStatementData(*statement));
	}

	/////////////////////////////////////////////////////////////////
	// Parsing functions
	/////////////////////////////////////////////////////////////////
	std::optional<Nodes::Expression*> Parser::ParseExpression()
	{
		if (PeekCheck(0, TokenType::Int64Literal))
			return Nodes::Expression::New(Nodes::Expression::Int64Literal::New(Consume()));

		else if (PeekCheck(0, TokenType::Identifier))
			return Nodes::Expression::New(Nodes::Expression::Identifier::New(Consume()));
			
		return {};
	}

	std::optional<Nodes::Statement*> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Exit statement
		/////////////////////////////////////////////////////////////////
		if (PeekCheck(0, TokenType::Exit) && PeekCheck(1, TokenType::OpenParenthesis))
		{
			Consume(); // Exit token
			Consume(); // '(' token

			Nodes::Statement::Exit* exitStatement = Nodes::Statement::Exit::New();

			// Expression resolution
			if (auto expr = ParseExpression())
				exitStatement->ExpressionObj = expr.value();
			else
				DY_LOG_ERROR("Invalid expression.");

			// Close parenthesis ')' & semicolon `;` resolution
			TryConsume(TokenType::CloseParenthesis, "Expected `)`.");
			TryConsume(TokenType::Semicolon, "Expected `;`.");

			return Nodes::Statement::New(exitStatement);
		}
		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (PeekCheck(0, TokenType::Let) && PeekCheck(1, TokenType::Identifier) && PeekCheck(2, TokenType::Equals))
		{
			Consume(); // 'let' token

			Nodes::Statement::Let* letStatement = Nodes::Statement::Let::New(Consume()); // Identifier token

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpression())
				letStatement->ExpressionObj = expr.value();
			else
				DY_LOG_ERROR("Invalid expression.");

			// Semicolon ';' resolution
			TryConsume(TokenType::Semicolon, "Expected `;`.");

			return Nodes::Statement::New(letStatement);
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

	Token Parser::TryConsume(TokenType tokenType, const std::string& msg)
	{
		if (PeekCheck(0, tokenType))
			return Consume();
		else if (!msg.empty())
			DY_LOG_ERROR(msg);

		return {};
	}

}
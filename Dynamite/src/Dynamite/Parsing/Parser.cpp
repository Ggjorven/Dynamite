#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

namespace Dynamite
{

	#define PeekCheck(offset, type) (Peek(offset).has_value() && Peek(offset).value().Type == type)

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Parser::Parser(Pulse::Memory::ArenaAllocator& allocator, const std::vector<Token>& tokens)
		: m_Allocator(allocator), m_Tokens(tokens)
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
			DY_LOG_TRACE("[{0}] - {1}", Nodes::StatementToString(statement), Nodes::FormatStatementData(statement));
	}

	/////////////////////////////////////////////////////////////////
	// Parsing functions
	/////////////////////////////////////////////////////////////////
	std::optional<Nodes::Expression> Parser::ParseExpression()
	{
		if (PeekCheck(0, TokenType::Int64Literal))
			return Nodes::Int64Literal { .TokenObject = Consume() };
		
		else if (PeekCheck(0, TokenType::Identifier))
			return Nodes::Identifier { .TokenObject = Consume() };
			
		return {};
	}

	std::optional<Nodes::Statement> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Exit statement
		/////////////////////////////////////////////////////////////////
		if (PeekCheck(0, TokenType::Exit) && PeekCheck(1, TokenType::OpenParenthesis))
		{
			Consume(); // Exit token
			Consume(); // '(' token

			Nodes::ExitStatement exitStatement = {};

			// Expression resolution
			if (auto expr = ParseExpression())
				exitStatement.ExpressionObject = expr.value();
			else
				DY_LOG_ERROR("Invalid expression.");

			// Close parenthesis ')' resolution
			if (PeekCheck(0, TokenType::CloseParenthesis)) 
				Consume(); // ')' token
			else 
				DY_LOG_ERROR("Expected `)`.");

			// Semicolon ';' resolution
			if (PeekCheck(0, TokenType::Semicolon))
				Consume();
			else 
				DY_LOG_ERROR("Expected `;`.");

			return exitStatement;
		}
		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (PeekCheck(0, TokenType::Let) && PeekCheck(1, TokenType::Identifier) && PeekCheck(2, TokenType::Equals))
		{
			Consume(); // 'let' token

			Nodes::LetStatement letStatement = Nodes::LetStatement { .TokenObject = Consume() }; // identifier token

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpression())
				letStatement.ExpressionObject = expr.value();
			else
				DY_LOG_ERROR("Invalid expression.");

			// Semicolon ';' resolution
			if (PeekCheck(0, TokenType::Semicolon))
				Consume();
			else 
				DY_LOG_ERROR("Expected `;`.");

			return letStatement;
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

}
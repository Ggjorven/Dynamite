#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

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
		return program;
	}

	/////////////////////////////////////////////////////////////////
	// Parsing functions
	/////////////////////////////////////////////////////////////////
	Optional<Node::Reference<Node::TermExpr>> Parser::ParseTermExpr()
	{
		//Utils::OptCheck(Peek(0), Token());

		auto o = Utils::OptReturnIfMemberIs(Peek(0), &Token::Type, GetAllTokenTypeLiterals());

		/*
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
		*/

		return {};
	}

	Optional<Node::Reference<Node::Expression>> Parser::ParseExpression(size_t minimumPrecedence)
	{
		return {};
	}

	Optional<Node::Reference<Node::ConditionBranch>> Parser::ParseConditionBrach()
	{
		return {};
	}

	Optional<Node::Reference<Node::IfStatement>> Parser::ParseIfStatement()
	{
		return {};
	}

	Optional<Node::Reference<Node::ScopeStatement>> Parser::ParseScopeStatement()
	{
		return {};
	}

	Optional<Node::Reference<Node::VariableStatement>> Parser::ParseVariableStatement()
	{
		return {};
	}

	Optional<Node::Reference<Node::Statement>> Parser::ParseStatement()
	{
		return {};
	}

	Optional<Node::Reference<Node::Function>> Parser::ParseFunction()
	{
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

	/////////////////////////////////////////////////////////////////
	// Other... // TODO
	/////////////////////////////////////////////////////////////////

}
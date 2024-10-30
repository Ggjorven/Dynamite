#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"
#include "Dynamite/Parser/Systems/ScopeSystem.hpp"
#include "Dynamite/Parser/Systems/FunctionSystem.hpp"

#include <Pulse/Core/Defines.hpp>

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

		m_Index = 0;

		TypeSystem::Reset();
		ScopeSystem::Reset();
		FunctionSystem::Reset();

		// Parse statements
		while (Peek(0).HasValue())
		{
			if (auto variableStatement = ParseVariableStatement())
				program.Definitions.emplace_back(variableStatement.Value());
			else if (auto function = ParseFunctionDefinition())
				program.Definitions.emplace_back(function.Value());
			else
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve a valid variable or function definition.");
				Consume(); // Consume a token, just to keep going.
			}
		}

		return program;
	}

	/////////////////////////////////////////////////////////////////
	// Peeking & consuming
	/////////////////////////////////////////////////////////////////
	Optional<Token> Parser::Peek(int64_t offset) const
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

	Optional<Type> Parser::GetType() 
	{
		Type result = {};

		// Front Qualifiers
		while (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeQualifiers()))
			result.FrontQualifiers.emplace_back(TokenTypeToTypeQualifier(Consume().Type));

		// Type
		{
			result.Information.Specifier = TokenTypeToTypeSpecifier(Consume().Type);

			if (result.Information.Specifier == TypeSpecifier::Identifier)
			{
				std::string name = Peek(-1).Value().Value;

				if (!TypeSystem::Exists(name))
					return {};
				
				result.Information.Value = name;
			}
		}

		// Back Qualifiers
		while (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeQualifiers()))
		{
			if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenSquareBracket) &&
				Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::IntegerLiteral) &&
				Utils::OptMemberIs(Peek(2), &Token::Type, TokenType::CloseSquareBracket))
			{
				Consume(); // '[' token
				auto size = Consume(); // IntegerLiteral token
				Consume(); // ']' token
				
				result.BackQualifiers.emplace_back(TypeQualifier::Array, size.Value);
			}
			else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenSquareBracket) &&
				Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::CloseSquareBracket))
			{
				Consume();
				Consume();

				result.BackQualifiers.emplace_back(TypeQualifier::Array);
			}
			else
				result.BackQualifiers.emplace_back(TokenTypeToTypeQualifier(Consume().Type));
		}

		return result;
	}

	void Parser::Cast(const Type& from, const Type& to, Node::Reference<Node::Expression> expression)
	{
		if (from == to)
			return;

		struct ExpressionVisitor
		{
			const Type& From;
			const Type& To;

			bool operator () (const Node::Reference<Node::TermExpr> obj) const
			{
				struct TermVisitor
				{
					const Type& From;
					const Type& To;

					bool operator () (const Node::Reference<Node::LiteralTerm> obj) const
					{
						return TypeSystem::Cast(From, To, obj->Literal.Value);;
					}
					bool operator () (const Node::Reference<Node::IdentifierTerm>) const
					{
						return false;
					}
					bool operator () (const Node::Reference<Node::ParenthesisTerm>) const
					{
						return false;
					}
				};

				return std::visit(TermVisitor(From, To), obj->Term);
			}
			bool operator () (const Node::Reference<Node::BinaryExpr>) const
			{
				return false;
			}
			bool operator () (const Node::Reference<Node::FunctionCall>) const
			{
				return false;
			}
			bool operator () (const Node::Reference<Node::AddressExpr>) const
			{
				return false;
			}
			bool operator () (const Node::Reference<Node::DereferenceExpr>) const
			{
				return false;
			}
		};

		std::string originalExprStr = Node::ExpressionToString(expression, 2);

		if (std::visit(ExpressionVisitor(from, to), expression->Expr))
			Compiler::Warn(Peek(0).Value().LineNumber, "Lost data while casting expression. From: {0}, to {1}\n    Original: \n{2} \n    New: \n{3}", TypeSystem::ToString(from), TypeSystem::ToString(to), originalExprStr, Node::ExpressionToString(expression, 2));
	}

}
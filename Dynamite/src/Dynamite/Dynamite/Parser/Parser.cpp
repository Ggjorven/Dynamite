#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

#include "Dynamite/Dynamite/Parser/Collections/ParserState.hpp"

#include "Dynamite/Dynamite/Parser/Collections/ParserScopeCollection.hpp"
#include "Dynamite/Dynamite/Parser/Collections/ParserFunctionCollection.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	namespace
	{
		static Parser* s_Instance = nullptr;
	}

	using namespace Language;
	using namespace Pulse::Enum::Bitwise;

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Parser::Parser(std::vector<Token>& tokens)
		: m_Tokens(tokens)
	{
		s_Instance = this;
	}

	Parser::~Parser()
	{
		s_Instance = nullptr;
	}

	Node::Program Parser::GetProgram()
	{
		Node::Program program = {};

		m_Index = 0;

		ParserScopeCollection::Reset();
		ParserFunctionCollection::Reset();

		// Parse statements
		while (Peek(0).HasValue())
		{
			// TODO: ...
			break;
		}

		DY_LOG_ERROR("Parser is currently disabled.");
		return program;
	}

	Parser& Parser::Get()
	{
		return *s_Instance;
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
		{
			if (Utils::OptMemberIs(Peek(offset), &Token::Type, TokenType::OpenSquareBracket) &&
				Utils::OptMemberIs(Peek(offset + 1), &Token::Type, TokenType::IntegerLiteral) &&
				Utils::OptMemberIs(Peek(offset + 2), &Token::Type, TokenType::CloseSquareBracket))
			{
				offset += 3;
			}
			else if (Utils::OptMemberIs(Peek(offset), &Token::Type, TokenType::OpenSquareBracket) &&
				Utils::OptMemberIs(Peek(offset + 1), &Token::Type, TokenType::CloseSquareBracket))
			{
				offset += 2;
			}
			else
				offset++;
		}

		return true;
	}

	Optional<Type> Parser::GetType() 
	{
		Type result = {};
		
		// Front Qualifiers
		while (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeQualifiers()))
			result.FrontQualifiers.Add(TokenTypeToTypeQualifier(Consume().Type));

		// Type
		{
			result.Information.Specifier = TokenTypeToTypeSpecifier(Consume().Type);

			if (result.Information.Specifier == TypeSpecifier::Identifier)
			{
				std::string name = Peek(-1).Value().Value;

				if (!TypeCollection::Exists(name))
					return {};
				
				result.Information.Value = name;
			}
		}

		// Back Qualifiers
		TypeQualifier qualifierBuffer = TypeQualifier::None;
		while (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeQualifiers()))
		{
			if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenSquareBracket) &&
				Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::IntegerLiteral) &&
				Utils::OptMemberIs(Peek(2), &Token::Type, TokenType::CloseSquareBracket))
			{
				Consume();				// '[' token
				auto size = Consume();	// IntegerLiteral token
				Consume();				// ']' token
				
				result.PushBack(qualifierBuffer | TypeQualifier::Array, std::stoull(size.Value));
				qualifierBuffer = TypeQualifier::None;
			}
			else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenSquareBracket) &&
				Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::CloseSquareBracket))
			{
				Consume();
				Consume();

				result.PushBack(qualifierBuffer | TypeQualifier::Array, 0);
				qualifierBuffer = TypeQualifier::None;
			}
			else
			{
				if (Peek(0).Value().Type == TokenType::Mut || Peek(0).Value().Type == TokenType::Volatile)
					qualifierBuffer |= TokenTypeToTypeQualifier(Consume().Type);
				else
				{
					result.PushBack(qualifierBuffer | TokenTypeToTypeQualifier(Consume().Type), 0);
					qualifierBuffer = TypeQualifier::None;
				}
			}
		}

		return result;
	}

}
#include "dypch.h"
#include "Token.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Tokens
	/////////////////////////////////////////////////////////////////
	Token::Token()
		: Type(TokenType::None), Value({}), LineNumber(0)
	{
	}

	Token::Token(TokenType type, uint32_t line)
		: Type(type), Value({}), LineNumber(line)
	{
	}

	Token::Token(TokenType type, std::optional<std::string> value, uint32_t line)
		: Type(type), Value(value), LineNumber(line)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string FormatToken(const Token& token)
	{
		if (token.Value.has_value())
			return Pulse::Text::Format("TokenType::{0}, Value = {1}", Pulse::Enum::Name(token.Type), token.Value.value());
		else
			return Pulse::Text::Format("TokenType::{0}", Pulse::Enum::Name(token.Type));
	}

}
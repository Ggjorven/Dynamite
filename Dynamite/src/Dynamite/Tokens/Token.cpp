#include "dypch.h"
#include "Token.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite
{

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

}
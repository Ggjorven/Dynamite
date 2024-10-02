#pragma once

#include <cstdint>

#include <string>
#include <optional>

namespace Dynamite
{

	enum class TokenType : uint16_t
	{
		None = 0,

		// Main
		Semicolon = ';',			// End of 'statement'
		OpenParenthesis = '(',		// '('
		CloseParenthesis = ')',		// ')'
		Equals = '=',

		Identifier = 1,				// Variable name?
		Let = 2,					// Variable declaration
		Exit = 3,					// Exit from Program

		// Types
		Int64Literal = 4,			// int64_t literal
	};

	struct Token
	{
	public:
		TokenType Type;
		std::optional<std::string> Value;

		uint32_t LineNumber;

	public:
		Token();
		Token(TokenType type, uint32_t line = 0);
		Token(TokenType type, std::optional<std::string> value, uint32_t line = 0);
		~Token() = default;
	};

}
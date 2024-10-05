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
		Equals = '=',				// '='

		// Operators
		Plus = '+',					// '+'
		Minus = '-',				// '-'
		Star = '*',					// '*'
		Divide = '/',				// '/'

		// ...
		Identifier = 1,				// Variable name
		IntegerLiteral,
		//FloatLiteral,
		//CharLiteral,
		//StringLiteral,

		Exit,						// Exit from Program

		// Types (Name is literally what can be used in program)
		//Int8,						// int8_t literal
		//Int16,						// int16_t literal
		//Int32,						// int32_t literal
		Int64,						// int64_t literal
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
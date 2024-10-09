#pragma once

#include <cstdint>

#include <string>
#include <optional>

namespace Dynamite
{

	enum class TokenType : uint8_t
	{
		None = 0,

		// Char tokens
		Semicolon = ';',			// End of 'statement'
		OpenParenthesis = '(',		// '('
		CloseParenthesis = ')',		// ')'
		OpenCurlyBrace = '{',		// '{'
		CloseCurlyBrace = '}',		// '}'
		Equals = '=',				// '='

		// Operators
		Plus = '+',					// '+'
		Add = Plus,
		Minus = '-',				// '-'
		Subtract = Minus,
		Star = '*',					// '*'
		Multiply = Star,
		ForwardSlash = '/',			// '/'
		Divide = ForwardSlash,

		Pipe = '|',
		Or = Pipe,
		And = '&',					// (Current lowest 38) Note: Keep in mind
		Xor = '^',

		// Main
		Identifier = 1,				// Variable/function name

		BoolLiteral,
		IntegerLiteral,
		FloatLiteral,
		CharLiteral,
		StringLiteral,

		// Keywords
		If,
		Else,

		Exit,						// Exit from Program

		// Types
		Bool,

		Int8,
		Int16,
		Int32,
		Int64,

		UInt8,
		UInt16,
		UInt32,
		UInt64,

		Float32,
		Float64,

		Char,
		String,
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

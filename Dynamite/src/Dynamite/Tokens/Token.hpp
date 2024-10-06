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

		Pipe = '|',
		Or = Pipe,
		And = '&',
		Xor = '^',

		// ...
		Identifier = 1,				// Variable name
		BoolLiteral,
		IntegerLiteral,
		FloatLiteral,
		CharLiteral,
		StringLiteral,

		Exit,						// Exit from Program

		// Types (Name is literally what can be used in program)
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
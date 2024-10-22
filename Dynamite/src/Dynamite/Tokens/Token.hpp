#pragma once

#include <cstdint>

#include <string>
#include <optional>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Tokens
	/////////////////////////////////////////////////////////////////
	enum class TokenType : uint8_t
	{
		None = 0,

		/////////////////////////////////////////////////////////////////
		// Char tokens
		/////////////////////////////////////////////////////////////////
		Semicolon,				// ';'

		OpenParenthesis,		// '('
		CloseParenthesis,		// ')'
		OpenCurlyBrace,			// '{'
		CloseCurlyBrace,		// '}'
		OpenSquareBracket,		// '['
		CloseSquareBracket,		// ']'

		Equals,					// '='
		Colon,					// ':'

		Period,					// '.'
		Dot = Period,			// '.', same as above.
		Comma,					// ','

		/////////////////////////////////////////////////////////////////
		// Operators
		/////////////////////////////////////////////////////////////////
		Plus,					// '+'
		Add = Plus,				// '+', same as above.
		Minus,					// '-'
		Subtract = Minus,		// '-', same as above.
		Star,					// '*'
		Multiply = Star,		// '*', same as above.
		ForwardSlash,			// '/'
		Divide = ForwardSlash,	// '/', same as above.

		Pipe,					// '|'
		Or = Pipe,				// '|', same as above.
		And,					// '&', same as above.
		Xor,					// '^' 
		UpArrow,				// '^', same as above.

		/////////////////////////////////////////////////////////////////
		// Literals
		/////////////////////////////////////////////////////////////////
		BoolLiteral,
		IntegerLiteral,
		FloatLiteral,
		CharLiteral,
		StringLiteral,

		/////////////////////////////////////////////////////////////////
		// Keywords
		/////////////////////////////////////////////////////////////////
		If,
		Else,

		Return,

		/////////////////////////////////////////////////////////////////
		// Specifiers
		/////////////////////////////////////////////////////////////////
		Pointer = Star,			// '*'
		Reference = And,		// '&'

		Const = (Return + 1),	// Note: Continue counting form return
		Volatile,

		/////////////////////////////////////////////////////////////////
		// Types
		/////////////////////////////////////////////////////////////////
		Void,

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

		// Note: An identifier can be a class name, a type name
		// a variable name or a function name.
		Identifier
	};

	/////////////////////////////////////////////////////////////////
	// Main struct
	/////////////////////////////////////////////////////////////////
	struct Token
	{
	public:
		TokenType Type = TokenType::None;
		
		// Note: Check if has value with .empty();
		std::string Value = {};

		// Note: This is for debugging and error messages.
		uint32_t LineNumber = -1;

	public:
		Token() = default;
		Token(TokenType type, uint32_t line = 0);
		Token(TokenType type, const std::string& value, uint32_t line = 0);
		~Token() = default;
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TokenTypeToString(TokenType type);
	std::string TokenToString(const Token& token);

}
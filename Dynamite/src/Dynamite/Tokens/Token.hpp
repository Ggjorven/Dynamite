#pragma once

#include <cstdint>

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

#include <Pulse/Core/Defines.hpp>

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
		CharArrayLiteral,

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

		Mut = (Return + 1),	// Note: Continue counting form return
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
		uint32_t LineNumber = Pulse::Numeric::Max<uint32_t>();

	public:
		Token() = default;
		Token(TokenType type, uint32_t line = 0);
		Token(TokenType type, const std::string& value, uint32_t line = 0);
		~Token() = default;

		Token(const Token& token);
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TokenTypeToString(TokenType type);
	std::string TokenToString(const Token& token);

	/////////////////////////////////////////////////////////////////
	// Type groups
	/////////////////////////////////////////////////////////////////
	inline constexpr const std::vector<TokenType> GetAllTokenTypeOperators()
	{
		return {
			TokenType::Add,
			TokenType::Subtract,
			TokenType::Multiply,
			TokenType::Divide,

			TokenType::Or,
			TokenType::And,
			TokenType::Xor,
		};
	}

	inline constexpr const std::vector<TokenType> GetAllTokenTypeLiterals()
	{
		return {
			TokenType::BoolLiteral,
			TokenType::IntegerLiteral,
			TokenType::FloatLiteral,
			TokenType::CharLiteral,
			TokenType::CharArrayLiteral,
		};
	}

	inline constexpr const std::vector<TokenType> GetAllTokenTypeQualifiers()
	{
		return {
			TokenType::Pointer,
			TokenType::Reference,

			TokenType::Mut,
			TokenType::Volatile,

			// For Array
			TokenType::OpenSquareBracket,
			TokenType::CloseSquareBracket,
		};
	}

	inline constexpr const std::vector<TokenType> GetAllTokenTypeTypes()
	{
		return {
			TokenType::Void,

			TokenType::Int8,
			TokenType::Int16,
			TokenType::Int32,
			TokenType::Int64,

			TokenType::UInt8,
			TokenType::UInt16,
			TokenType::UInt32,
			TokenType::UInt64,

			TokenType::Float32,
			TokenType::Float64,

			TokenType::Char,

			TokenType::Identifier,
		};
	}

}
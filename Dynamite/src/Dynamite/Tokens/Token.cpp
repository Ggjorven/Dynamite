#include "dypch.h"
#include "Token.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Main struct
	/////////////////////////////////////////////////////////////////
	Token::Token(TokenType type, uint32_t line)
		: Type(type), Value({}), LineNumber(line)
	{
	}

	Token::Token(TokenType type, const std::string& value, uint32_t line)
		: Type(type), Value(value), LineNumber(line)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	// Note: Has to manually updated.
	std::string TokenTypeToString(TokenType type)
	{
		switch (type)
		{
		/////////////////////////////////////////////////////////////////
		// Char tokens
		/////////////////////////////////////////////////////////////////
		case TokenType::Semicolon:				return ";";

		case TokenType::OpenParenthesis:		return "(";		
		case TokenType::CloseParenthesis:		return ")";		
		case TokenType::OpenCurlyBrace:			return "{";		
		case TokenType::CloseCurlyBrace:		return "}";		
		case TokenType::OpenSquareBracket:		return "[";
		case TokenType::CloseSquareBracket:		return "]";

		case TokenType::Equals:					return "=";		
		case TokenType::Colon:					return ":";		

		case TokenType::Period:					return ".";		
		case TokenType::Comma:					return ",";		

		/////////////////////////////////////////////////////////////////
		// Operators
		/////////////////////////////////////////////////////////////////
		case TokenType::Plus:					return "+";			
		case TokenType::Minus:					return "-";
		case TokenType::Star:					return "*";
		case TokenType::ForwardSlash:			return "/";
		
		case TokenType::Pipe:					return "|";			
		case TokenType::And:					return "&";			
		case TokenType::UpArrow:				return "^";			

		/////////////////////////////////////////////////////////////////
		// Literals
		/////////////////////////////////////////////////////////////////
		case TokenType::BoolLiteral:			return "<unnameable token>";
		case TokenType::IntegerLiteral:			return "<unnameable token>";
		case TokenType::FloatLiteral:			return "<unnameable token>";
		case TokenType::CharLiteral:			return "<unnameable token>";
		case TokenType::CharArrayLiteral:		return "<unnameable token>";

		/////////////////////////////////////////////////////////////////
		// Keywords
		/////////////////////////////////////////////////////////////////
		case TokenType::If:						return "if";
		case TokenType::Else:					return "else";

		case TokenType::Return:					return "return";

		/////////////////////////////////////////////////////////////////
		// Specifiers
		/////////////////////////////////////////////////////////////////
		case TokenType::Const:					return "const";
		case TokenType::Volatile:				return "volatile";

		/////////////////////////////////////////////////////////////////
		// Types
		/////////////////////////////////////////////////////////////////
		case TokenType::Void:					return "void";

		case TokenType::Bool:					return "bool";

		case TokenType::Int8:					return "i8";
		case TokenType::Int16:					return "i16";
		case TokenType::Int32:					return "i32";
		case TokenType::Int64:					return "i64";

		case TokenType::UInt8:					return "u8";
		case TokenType::UInt16:					return "u16";
		case TokenType::UInt32:					return "u32";
		case TokenType::UInt64:					return "u64";

		case TokenType::Float32:				return "f32";
		case TokenType::Float64:				return "f64";

		case TokenType::Char:					return "char";

		// Note: An identifier can be a class name, a type name
		// a variable name or a function name.
		case TokenType::Identifier:				return "<unnameable token>";

		default:
			break;
		}

		return "<undefined token name>";
	}

	std::string TokenToString(const Token& token)
	{
		std::string str = {};

		str += Pulse::Text::Format("([TokenType::{0}]", Pulse::Enum::Name(token.Type));

		if (!token.Value.empty())
			str += Pulse::Text::Format(" = '{0}'", token.Value);
		
		str += ')';

		return str;
	}

}
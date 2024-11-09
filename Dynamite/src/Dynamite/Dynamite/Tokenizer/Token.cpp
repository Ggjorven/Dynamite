#include "dypch.h"
#include "Token.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

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

	Token::Token(const Token& token)
		: Type(token.Type), Value(token.Value), LineNumber(token.LineNumber)
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

		case TokenType::TakeAddress:			return "#";

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
		// Qualifiers
		/////////////////////////////////////////////////////////////////
		case TokenType::Mut:					return Language::TypeQualifierToString(Language::TypeQualifier::Mut);
		case TokenType::Volatile:				return Language::TypeQualifierToString(Language::TypeQualifier::Volatile);

		/////////////////////////////////////////////////////////////////
		// Specifiers
		/////////////////////////////////////////////////////////////////
		case TokenType::Void:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Void);

		case TokenType::Bool:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Bool);

		case TokenType::Int8:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Int8);
		case TokenType::Int16:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Int16);
		case TokenType::Int32:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Int32);
		case TokenType::Int64:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Int64);

		case TokenType::UInt8:					return Language::TypeSpecifierToString(Language::TypeSpecifier::UInt8);
		case TokenType::UInt16:					return Language::TypeSpecifierToString(Language::TypeSpecifier::UInt16);
		case TokenType::UInt32:					return Language::TypeSpecifierToString(Language::TypeSpecifier::UInt32);
		case TokenType::UInt64:					return Language::TypeSpecifierToString(Language::TypeSpecifier::UInt64);

		case TokenType::Float32:				return Language::TypeSpecifierToString(Language::TypeSpecifier::Float32);
		case TokenType::Float64:				return Language::TypeSpecifierToString(Language::TypeSpecifier::Float64);

		case TokenType::Char:					return Language::TypeSpecifierToString(Language::TypeSpecifier::Char);

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

	/////////////////////////////////////////////////////////////////
	// Conversions
	/////////////////////////////////////////////////////////////////
	Language::LiteralType TokenTypeToLiteralType(TokenType tokenType)
	{
		if (!Utils::IsEqual(tokenType, GetAllTokenTypeLiterals()))
		{
			DY_LOG_ERROR("Tried to get LiteralType from literal, but TokenType::{0} is not a literal.", Pulse::Enum::Name(tokenType));
			return {};
		}

		switch (tokenType)
		{
		case TokenType::BoolLiteral:		return Language::LiteralType::BoolLiteral;

		case TokenType::IntegerLiteral:		return Language::LiteralType::IntegerLiteral;

		case TokenType::FloatLiteral:		return Language::LiteralType::FloatLiteral;

		case TokenType::CharLiteral:		return Language::LiteralType::CharLiteral;
		case TokenType::CharArrayLiteral:	return Language::LiteralType::CharArrayLiteral;

		default:
			break;
		}

		DY_LOG_ERROR("[Internal Compiler Error] TokenType::{0} passed == literal check, but did not return a valid LiteralType.", Pulse::Enum::Name(tokenType));
		return Language::LiteralType::None;
	}

	Language::OperationType TokenTypeToOperationType(TokenType tokenType)
	{
		if (!Utils::IsEqual(tokenType, GetAllTokenTypeOperators()))
		{
			DY_LOG_ERROR("Tried to get OperationType from operation, but TokenType::{0} is not a operation.", Pulse::Enum::Name(tokenType));
			return {};
		}

		switch (tokenType)
		{
		case TokenType::Add:			return Language::OperationType::Add;
		case TokenType::Subtract:		return Language::OperationType::Subtract;
		case TokenType::Multiply:		return Language::OperationType::Multiply;
		case TokenType::Divide:			return Language::OperationType::Divide;

		case TokenType::Or:				return Language::OperationType::OR;
		case TokenType::And:			return Language::OperationType::AND;
		case TokenType::Xor:			return Language::OperationType::XOR;

		default:
			break;
		}

		DY_LOG_ERROR("[Internal Compiler Error] TokenType::{0} passed == operation check, but did not return a valid OperationType.", Pulse::Enum::Name(tokenType));
		return Language::OperationType::None;
	}

	Language::TypeQualifier TokenTypeToTypeQualifier(TokenType tokenType)
	{
		if (!Utils::IsEqual(tokenType, GetAllTokenTypeQualifiers()))
		{
			DY_LOG_ERROR("Tried to get TypeQualifier from TokenType, but TokenType::{0} is not a qualifier.", Pulse::Enum::Name(tokenType));
			return {};
		}

		switch (tokenType)
		{
		case TokenType::Mut:				return Language::TypeQualifier::Mut;
		case TokenType::Volatile:			return Language::TypeQualifier::Volatile;

		case TokenType::Pointer:			return Language::TypeQualifier::Pointer;
		case TokenType::Reference:			return Language::TypeQualifier::Reference;

		default:
			break;
		}

		DY_LOG_ERROR("[Internal Compiler Error] TokenType::{0} passed == qualifier check, but did not return a valid TypeQualifier.", Pulse::Enum::Name(tokenType));
		return Language::TypeQualifier::None;
	}

	Language::TypeSpecifier TokenTypeToTypeSpecifier(TokenType tokenType)
	{
		if (!Utils::IsEqual(tokenType, GetAllTokenTypeTypes()))
		{
			DY_LOG_ERROR("Tried to get TypeSpecifier from TokenType, but TokenType::{0} is not a specifier.", Pulse::Enum::Name(tokenType));
			return {};
		}

		switch (tokenType)
		{
		case TokenType::Void:				return Language::TypeSpecifier::Void;

		case TokenType::Bool:				return Language::TypeSpecifier::Bool;

		case TokenType::Int8:				return Language::TypeSpecifier::Int8;
		case TokenType::Int16:				return Language::TypeSpecifier::Int16;
		case TokenType::Int32:				return Language::TypeSpecifier::Int32;
		case TokenType::Int64:				return Language::TypeSpecifier::Int64;

		case TokenType::UInt8:				return Language::TypeSpecifier::UInt8;
		case TokenType::UInt16:				return Language::TypeSpecifier::UInt16;
		case TokenType::UInt32:				return Language::TypeSpecifier::UInt32;
		case TokenType::UInt64:				return Language::TypeSpecifier::UInt64;

		case TokenType::Float32:			return Language::TypeSpecifier::Float32;
		case TokenType::Float64:			return Language::TypeSpecifier::Float64;

		case TokenType::Char:				return Language::TypeSpecifier::Char;

		case TokenType::Identifier:			return Language::TypeSpecifier::Identifier;

		default:
			break;
		}

		DY_LOG_ERROR("[Internal Compiler Error] TokenType::{0} passed == specifier check, but did not return a valid TypeSpecifier.", Pulse::Enum::Name(tokenType));
		return Language::TypeSpecifier::None;
	}

}
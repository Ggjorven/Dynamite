#include "dypch.h"
#include "Type.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Type Information
	/////////////////////////////////////////////////////////////////
	TypeInfo::TypeInfo(TypeSpecifier specifier, const std::string& value)
		: Specifier(specifier), Value(value)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeQualifierToString(TypeQualifier qualifier)
	{
		switch (qualifier)
		{
		case TypeQualifier::Const:		return TokenTypeToString(TokenType::Const);
		case TypeQualifier::Volatile:	return TokenTypeToString(TokenType::Volatile);

		case TypeQualifier::Pointer:	return TokenTypeToString(TokenType::Pointer);
		case TypeQualifier::Reference:	return TokenTypeToString(TokenType::Reference);

		default:
			break;
		}

		return "<undefined qualifier name>";
	}

	std::string TypeSpecifierToString(TypeSpecifier specifier)
	{
		switch (specifier)
		{
		case TypeSpecifier::Void:		return TokenTypeToString(TokenType::Void);

		case TypeSpecifier::Bool:		return TokenTypeToString(TokenType::Bool);

		case TypeSpecifier::Int8:		return TokenTypeToString(TokenType::Int8);
		case TypeSpecifier::Int16:		return TokenTypeToString(TokenType::Int16);
		case TypeSpecifier::Int32:		return TokenTypeToString(TokenType::Int32);
		case TypeSpecifier::Int64:		return TokenTypeToString(TokenType::Int64);

		case TypeSpecifier::UInt8:		return TokenTypeToString(TokenType::UInt8);
		case TypeSpecifier::UInt16:		return TokenTypeToString(TokenType::UInt16);
		case TypeSpecifier::UInt32:		return TokenTypeToString(TokenType::UInt32);
		case TypeSpecifier::UInt64:		return TokenTypeToString(TokenType::UInt64);

		case TypeSpecifier::Float32:	return TokenTypeToString(TokenType::Float32);
		case TypeSpecifier::Float64:	return TokenTypeToString(TokenType::Float64);

		case TypeSpecifier::Char:		return TokenTypeToString(TokenType::Char);
		case TypeSpecifier::String:		return TokenTypeToString(TokenType::String);

		// Note: A custom class or type is also an identifier.
		case TypeSpecifier::Identifier:	return "<unnameable specifier>";

		default:
			break;
		}

		return "<undefined specifier name>";

	}

	std::string TypeToString(const Type& type)
	{
		std::string str = {};

		for (const auto& qualifier : type.FrontQualifiers)
			str += TypeQualifierToString(qualifier) + ' ';

		if (type.Information.Specifier == TypeSpecifier::Identifier)
			str += type.Information.Value;
		else
			str += TypeSpecifierToString(type.Information.Specifier);

		for (const auto& qualifier : type.BackQualifiers)
			str += TypeQualifierToString(qualifier);

		return "<undefined type name>";
	}

}
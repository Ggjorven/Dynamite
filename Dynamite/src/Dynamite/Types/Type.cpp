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

	bool TypeInfo::operator == (const TypeInfo& other)
	{
		bool result = this->Specifier == other.Specifier;
		result |= this->Value == other.Value;

		return result;
	}

	bool TypeInfo::operator != (const TypeInfo& other)
	{
		return !(*this == other);
	}

	/////////////////////////////////////////////////////////////////
	// Type
	/////////////////////////////////////////////////////////////////
	// Constructors
	Type::Type(const TypeInfo& info)
		: Information(info)
	{
	}

	Type::Type(const std::vector<TypeQualifier>& front, const TypeInfo& info, const std::vector<TypeQualifier>& back)
		: FrontQualifiers(front), Information(info), BackQualifiers(back)
	{
	}

	// Operators
	bool Type::operator == (const Type& other)
	{
		// Check if the qualifier counts are the same.
		if ((this->FrontQualifiers.size() != other.FrontQualifiers.size()) 
			|| (this->BackQualifiers.size() != other.BackQualifiers.size()))
			return false;

		// Type information
		if (this->Information != other.Information)
			return false;

		// Front Qualifiers
		for (size_t i = 0; i < this->FrontQualifiers.size(); i++)
		{
			if (this->FrontQualifiers[i] != other.FrontQualifiers[i])
				return false;
		}

		// Back Qualifiers
		for (size_t i = 0; i < this->BackQualifiers.size(); i++)
		{
			if (this->BackQualifiers[i] != other.BackQualifiers[i])
				return false;
		}

		return false;
	}

	bool Type::operator != (const Type& other)
	{
		return !(*this == other);
	}

	bool Type::operator == (const TypeInfo& info)
	{
		return (this->Information == info);
	}

	bool Type::operator != (const TypeInfo& info)
	{
		return !(*this == info);
	}

	// Methods
	bool Type::IsConst()
	{
		for (const auto& qualifier : FrontQualifiers)
		{
			if (qualifier == TypeQualifier::Const)
				return true;
		}

		return false;
	}

	bool Type::IsVolatile()
	{
		for (const auto& qualifier : FrontQualifiers)
		{
			if (qualifier == TypeQualifier::Volatile)
				return true;
		}

		return false;
	}

	bool Type::IsPointer()
	{
		return (BackQualifiers.back() == TypeQualifier::Pointer);
	}

	bool Type::IsReference()
	{
		return (BackQualifiers.back() == TypeQualifier::Reference);
	}

	bool Type::IsArray()
	{
		return (BackQualifiers.back() == TypeQualifier::Array);
	}

	Type Type::Clean()
	{
		// No front qualifiers (const, volatile)
		Type clean = Type({}, this->Information, this->BackQualifiers);

		// Remove reference if it is the last qualifier
		if (clean.BackQualifiers.back() == TypeQualifier::Reference)
			clean.BackQualifiers.pop_back();

		return clean;
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
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

		// Note: A custom class or type is also an identifier.
		case TypeSpecifier::Identifier:	return "<unnameable specifier>";

		default:
			break;
		}

		return "<undefined specifier name>";

	}

	std::string TypeQualifierToString(TypeQualifier qualifier)
	{
		switch (qualifier)
		{
		case TypeQualifier::Const:		return TokenTypeToString(TokenType::Const);
		case TypeQualifier::Volatile:	return TokenTypeToString(TokenType::Volatile);

		case TypeQualifier::Pointer:	return TokenTypeToString(TokenType::Pointer);
		case TypeQualifier::Reference:	return TokenTypeToString(TokenType::Reference);

		case TypeQualifier::Array:		return "[]";

		default:
			break;
		}

		return "<undefined qualifier name>";
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

		return str;
	}

}
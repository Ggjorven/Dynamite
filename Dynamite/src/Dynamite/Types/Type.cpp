#include "dypch.h"
#include "Type.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

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

	bool TypeInfo::operator == (const TypeInfo& other) const
	{
		bool result = this->Specifier == other.Specifier;
		result |= this->Value == other.Value;

		return result;
	}

	bool TypeInfo::operator != (const TypeInfo& other) const
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
	bool Type::operator == (const Type& other) const
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

	bool Type::operator != (const Type& other) const
	{
		return !(*this == other);
	}

	bool Type::operator == (const TypeInfo& info) const
	{
		return (this->Information == info);
	}

	bool Type::operator != (const TypeInfo& info) const
	{
		return !(*this == info);
	}

	// Methods
	TypeQualifier Type::CleanBackQualifier() const
	{
		if (BackQualifiers.empty())
			return TypeQualifier::None;

		if (BackQualifiers.size() >= 2)
		{
			if (BackQualifiers.back() == TypeQualifier::Reference)
				return BackQualifiers.at(BackQualifiers.size() - 2);
		}

		return BackQualifiers.back();
	}

	bool Type::IsConst() const
	{
		for (const auto& qualifier : FrontQualifiers)
		{
			if (qualifier == TypeQualifier::Const)
				return true;
		}

		return false;
	}

	bool Type::IsVolatile() const
	{
		for (const auto& qualifier : FrontQualifiers)
		{
			if (qualifier == TypeQualifier::Volatile)
				return true;
		}

		return false;
	}

	bool Type::IsPointer() const
	{
		return (CleanBackQualifier() == TypeQualifier::Pointer);
	}

	bool Type::IsReference() const
	{
		if (!BackQualifiers.empty() && BackQualifiers.back() == TypeQualifier::Reference)
			return true;

		return false;
	}

	bool Type::IsArray() const
	{
		return (CleanBackQualifier() == TypeQualifier::Array);
	}

	Type Type::Clean() const
	{
		// No front qualifiers (const, volatile)
		Type clean = Type({}, this->Information, this->BackQualifiers);

		// Remove reference if it is the last qualifier
		if (!clean.BackQualifiers.empty() && clean.BackQualifiers.back() == TypeQualifier::Reference)
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

	TypeSpecifier TokenTypeToTypeSpecifier(TokenType tokenType)
	{
		if (!Utils::IsEqual(tokenType, GetAllTokenTypeTypes()))
		{
			DY_LOG_WARN("Tried to get TypeSpecifier from TokenType, but TokenType::{0} is not a specifier.", Pulse::Enum::Name(tokenType));
			return {};
		}

		switch (tokenType)
		{
		case TokenType::Void:				return TypeSpecifier::Void;

		case TokenType::Int8:				return TypeSpecifier::Int8;
		case TokenType::Int16:				return TypeSpecifier::Int16;
		case TokenType::Int32:				return TypeSpecifier::Int32;
		case TokenType::Int64:				return TypeSpecifier::Int64;

		case TokenType::UInt8:				return TypeSpecifier::UInt8;
		case TokenType::UInt16:				return TypeSpecifier::UInt16;
		case TokenType::UInt32:				return TypeSpecifier::UInt32;
		case TokenType::UInt64:				return TypeSpecifier::UInt64;

		case TokenType::Float32:			return TypeSpecifier::Float32;
		case TokenType::Float64:			return TypeSpecifier::Float64;

		case TokenType::Char:				return TypeSpecifier::Char;

		case TokenType::Identifier:			return TypeSpecifier::Identifier;

		default:
			break;
		}

		return TypeSpecifier::None;
	}

	TypeQualifier TokenTypeToTypeQualifier(TokenType tokenType)
	{
		if (!Utils::IsEqual(tokenType, GetAllTokenTypeQualifiers()))
		{
			DY_LOG_WARN("Tried to get TypeQualifier from TokenType, but TokenType::{0} is not a qualifier.", Pulse::Enum::Name(tokenType));
			return {};
		}

		switch (tokenType)
		{
		case TokenType::Const:				return TypeQualifier::Const;
		case TokenType::Volatile:			return TypeQualifier::Volatile;

		case TokenType::Pointer:			return TypeQualifier::Pointer;
		case TokenType::Reference:			return TypeQualifier::Reference;

		default:
			break;
		}

		return TypeQualifier::None;
	}

}
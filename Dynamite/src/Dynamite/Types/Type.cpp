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
		bool result = (this->Specifier == other.Specifier);

		if (result && this->Specifier == TypeSpecifier::Identifier)
		{
			if (this->Value != other.Value)
				result = false;
		}

		return result;
	}

	bool TypeInfo::operator != (const TypeInfo& other) const
	{
		return !(*this == other);
	}

	/////////////////////////////////////////////////////////////////
	// Type
	/////////////////////////////////////////////////////////////////
	// QualifierPack operators
	bool Type::QualifierPack::operator == (const TypeQualifier& qualifier) const
	{
		return (this->Qualifier == qualifier);
	}

	bool Type::QualifierPack::operator != (const TypeQualifier& qualifier) const
	{
		return !(*this == qualifier);
	}

	bool Type::QualifierPack::operator == (const QualifierPack& other) const
	{
		if ((this->Value == other.Value) &&
			(this->Qualifier == other.Qualifier))
			return true;

		return false;
	}

	bool Type::QualifierPack::operator != (const QualifierPack& other) const
	{
		return !(*this == other);
	}
	
	// Constructors
	Type::Type(const TypeInfo& info)
		: Information(info)
	{
	}

	Type::Type(const std::vector<QualifierPack>& front, const TypeInfo& info, const std::vector<QualifierPack>& back)
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

		return true;
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

	// Checks
	bool Type::IsMut() const
	{
		for (const auto& qualifier : FrontQualifiers)
		{
			if (qualifier == TypeQualifier::Mut)
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
		return ((!BackQualifiers.empty()) && (BackQualifiers.back() == TypeQualifier::Pointer));
	}

	bool Type::IsArray() const
	{
		return ((!BackQualifiers.empty()) && (BackQualifiers.back() == TypeQualifier::Array));
	}

	// Adds
	void Type::AddMut()
	{
		if (!IsMut())
			FrontQualifiers.emplace_back(TypeQualifier::Mut);
	}

	void Type::AddVolatile()
	{
		if (!IsVolatile())
			FrontQualifiers.emplace_back(TypeQualifier::Mut);
	}

	void Type::AddPointer()
	{
		BackQualifiers.emplace_back(TypeQualifier::Pointer);
	}

	void Type::AddArray(const std::string& size)
	{
		BackQualifiers.emplace_back(TypeQualifier::Array, size);
	}

	std::string Type::GetArraySize()
	{
		if (!IsArray())
			return {};

		return BackQualifiers.back().Value;
	}

	void Type::SetArraySize(const std::string& value)
	{
		if (!IsArray())
		{
			DY_LOG_WARN("Trying to set array size, when Type is not an array.");
			return;
		}

		BackQualifiers.back().Value = value;
	}

	Type Type::Clean() const
	{
		return Type({}, this->Information, this->BackQualifiers);
	}

	Type Type::Copy() const
	{
		return Type(this->FrontQualifiers, this->Information, this->BackQualifiers);
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

	std::string TypeQualifierToString(TypeQualifier qualifier, const std::string& value)
	{
		switch (qualifier)
		{
		case TypeQualifier::Mut:		return TokenTypeToString(TokenType::Mut);
		case TypeQualifier::Volatile:	return TokenTypeToString(TokenType::Volatile);

		case TypeQualifier::Pointer:	return TokenTypeToString(TokenType::Pointer);
		//case TypeQualifier::Reference:	return TokenTypeToString(TokenType::Reference);

		case TypeQualifier::Array:		return std::string("[") + value + std::string("]");

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
		case TokenType::Mut:				return TypeQualifier::Mut;
		case TokenType::Volatile:			return TypeQualifier::Volatile;

		case TokenType::Pointer:			return TypeQualifier::Pointer;
		//case TokenType::Reference:			return TypeQualifier::Reference;

		default:
			break;
		}

		return TypeQualifier::None;
	}

}
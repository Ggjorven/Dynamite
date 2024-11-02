#include "dypch.h"
#include "Type.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Information structs
	/////////////////////////////////////////////////////////////////
	// Constructors
	TypeInfo::TypeInfo(TypeSpecifier specifier, const std::string& value)
		: Specifier(specifier), Value(value)
	{
	}

	// Operators
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

	// Constructors
	QualifierInfo::QualifierInfo(TypeQualifier qualifier, const std::string& value)
		: Qualifier(qualifier), Value(value)
	{
	}

	// Operators
	bool QualifierInfo::operator == (const TypeQualifier& qualifier) const
	{
		return (this->Qualifier == qualifier);
	}

	bool QualifierInfo::operator != (const TypeQualifier& qualifier) const
	{
		return !(*this == qualifier);
	}

	bool QualifierInfo::operator == (const QualifierInfo& other) const
	{
		if ((this->Value == other.Value) &&
			(this->Qualifier == other.Qualifier))
			return true;

		return false;
	}

	bool QualifierInfo::operator != (const QualifierInfo& other) const
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

	Type::Type(const std::vector<QualifierInfo>& front, const TypeInfo& info, const std::vector<QualifierInfo>& back)
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

	bool Type::IsUnsigned() const
	{
		if (Information.Specifier == TypeSpecifier::UInt8 ||
			Information.Specifier == TypeSpecifier::UInt16 ||
			Information.Specifier == TypeSpecifier::UInt32 ||
			Information.Specifier == TypeSpecifier::UInt64)
		{
			return true;
		}

		return false;
	}

	// Type checks
	bool Type::IsVoid() const
	{
		if (IsPointer())
			return false;

		return Information.Specifier == TypeSpecifier::Void;
	}

	bool Type::IsBool() const
	{
		if (IsPointer())
			return false;

		if (Information.Specifier == TypeSpecifier::Bool)
		{
			return true;
		}

		return false;
	}

	bool Type::IsIntegral() const
	{
		if (IsPointer())
			return false;

		if (Information.Specifier == TypeSpecifier::Int8 ||
			Information.Specifier == TypeSpecifier::Int16 ||
			Information.Specifier == TypeSpecifier::Int32 ||
			Information.Specifier == TypeSpecifier::Int64 ||

			Information.Specifier == TypeSpecifier::UInt8 ||
			Information.Specifier == TypeSpecifier::UInt16 ||
			Information.Specifier == TypeSpecifier::UInt32 ||
			Information.Specifier == TypeSpecifier::UInt64)
		{
			return true;
		}

		return false;
	}

	bool Type::IsFloat() const
	{
		if (IsPointer())
			return false;

		if (Information.Specifier == TypeSpecifier::Float32 ||
			Information.Specifier == TypeSpecifier::Float64)
		{
			return true;
		}

		return false;
	}

	bool Type::IsChar() const
	{
		if (IsPointer())
			return false;

		if (Information.Specifier == TypeSpecifier::Char)
		{
			return true;
		}

		return false;
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

	std::string Type::GetArraySize() const
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

	// Utils
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
		case TypeSpecifier::Void:		return "void";

		case TypeSpecifier::Bool:		return "bool";

		case TypeSpecifier::Int8:		return "i8";
		case TypeSpecifier::Int16:		return "i16";
		case TypeSpecifier::Int32:		return "i32";
		case TypeSpecifier::Int64:		return "i64";

		case TypeSpecifier::UInt8:		return "u8";
		case TypeSpecifier::UInt16:		return "u16";
		case TypeSpecifier::UInt32:		return "u32";
		case TypeSpecifier::UInt64:		return "u64";

		case TypeSpecifier::Float32:	return "f32";
		case TypeSpecifier::Float64:	return "f64";

		case TypeSpecifier::Char:		return "char";

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
		case TypeQualifier::Mut:		return "mut";
		case TypeQualifier::Volatile:	return "volatile";

		case TypeQualifier::Pointer:	return "*";

		case TypeQualifier::Array:		return "[" + value + "]";

		default:
			break;
		}

		return "<undefined qualifier name>";
	}

}
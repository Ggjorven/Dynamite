#include "dypch.h"
#include "Type.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language
{

	using namespace Pulse::Enum::Bitwise;

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

	/////////////////////////////////////////////////////////////////
	// QualifierGroup
	/////////////////////////////////////////////////////////////////
	// Constructors
	QualifierGroup::QualifierGroup(TypeQualifier qualifiers, size_t arraySize)
		: Qualifiers(qualifiers), ArraySize(arraySize)
	{
	}

	// Operators
	bool QualifierGroup::operator == (const QualifierGroup& other) const
	{
		return ((this->Qualifiers == other.Qualifiers) && this->ArraySize == other.ArraySize);
	}

	bool QualifierGroup::operator != (const QualifierGroup& other) const
	{
		return !(*this == other);
	}

	// Functions
	void QualifierGroup::Add(TypeQualifier qualifier)
	{
		Qualifiers |= qualifier;
	}

	bool QualifierGroup::Contains(TypeQualifier qualifier) const
	{
		return (bool)(Qualifiers & qualifier);
	}

	// Qualifier checks
	bool QualifierGroup::IsMut() const
	{
		return Contains(TypeQualifier::Mut);
	}

	bool QualifierGroup::IsVolatile() const
	{
		return Contains(TypeQualifier::Volatile);
	}

	bool QualifierGroup::IsPointer() const
	{
		return Contains(TypeQualifier::Pointer);
	}

	bool QualifierGroup::IsReference() const
	{
		return Contains(TypeQualifier::Reference);
	}

	bool QualifierGroup::IsArray() const
	{
		return Contains(TypeQualifier::Array);
	}

	std::vector<TypeQualifier> QualifierGroup::SplitQualifiers() const
	{
		std::vector<TypeQualifier> result;

		if (IsMut())		result.emplace_back(TypeQualifier::Mut);
		if (IsVolatile())	result.emplace_back(TypeQualifier::Volatile);
		if (IsPointer())	result.emplace_back(TypeQualifier::Pointer);
		if (IsReference())	result.emplace_back(TypeQualifier::Reference);
		if (IsArray())		result.emplace_back(TypeQualifier::Array);

		return result;
	}

	/////////////////////////////////////////////////////////////////
	// Type
	/////////////////////////////////////////////////////////////////
	// Constructors
	Type::Type(const TypeInfo& info)
		: Information(info)
	{
	}

	Type::Type(QualifierGroup front, const TypeInfo& info, const std::vector<QualifierGroup>& back)
		: FrontQualifiers(front), Information(info), BackQualifiers(back)
	{
	}

	// Operators
	bool Type::operator == (const Type& other) const
	{
		// Check if the qualifier counts are the same.
		if ((this->BackQualifiers.size() != other.BackQualifiers.size()))
			return false;

		// Type information
		if (this->Information != other.Information)
			return false;

		// Front Qualifiers
		if (this->FrontQualifiers != other.FrontQualifiers)
			return false;

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
		return FrontQualifiers.IsMut();
	}

	bool Type::IsVolatile() const
	{
		return FrontQualifiers.IsVolatile();
	}

	bool Type::IsPointer() const
	{
		return ((!BackQualifiers.empty()) && (BackQualifiers.back().IsPointer()));
	}

	bool Type::IsReference() const
	{
		return ((!BackQualifiers.empty()) && (BackQualifiers.back().IsReference()));
	}

	bool Type::IsArray() const
	{
		return ((!BackQualifiers.empty()) && (BackQualifiers.back().IsArray()));
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

	bool Type::IsSigned() const
	{
		return !(IsUnsigned());
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

	size_t Type::GetArraySize() const
	{
		if (!IsArray())
			return 0;

		return BackQualifiers.back().GetArraySize();
	}

	void Type::SetArraySize(size_t size)
	{
		if (!IsArray())
			return;

		BackQualifiers.back().SetArraySize(size);
	}

	void Type::AddToBack(TypeQualifier qualifier, Optional<size_t> arraySize)
	{
		if (BackQualifiers.empty())
			BackQualifiers.emplace_back();

		BackQualifiers.back().Add(qualifier);

		if (arraySize.HasValue())
			BackQualifiers.back().SetArraySize(arraySize.Value());
	}

	void Type::PopBack()
	{
		if (BackQualifiers.empty())
			return;

		BackQualifiers.pop_back();
	}

	// Utils
	Type Type::RemoveReference() const
	{
		Type ret = Type(this->FrontQualifiers, this->Information, this->BackQualifiers);

		if (IsReference())
			ret.BackQualifiers.pop_back();

		return ret;
	}

	Type Type::RemovePointer() const
	{
		Type ret = Type(this->FrontQualifiers, this->Information, this->BackQualifiers);

		if (IsPointer())
			ret.BackQualifiers.pop_back();

		return ret;
	}

	Type Type::Clean() const
	{
		return Type({}, this->Information, this->BackQualifiers);
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

	std::string TypeQualifierToString(TypeQualifier qualifier, size_t arraySize)
	{
		switch (qualifier)
		{
		case TypeQualifier::Mut:		return "mut";
		case TypeQualifier::Volatile:	return "volatile";

		case TypeQualifier::Reference:	return "&";
		case TypeQualifier::Pointer:	return "*";

		case TypeQualifier::Array:		return "[" + std::to_string(arraySize) + "]";

		default:
			break;
		}

		return "<undefined qualifier name>";
	}

}
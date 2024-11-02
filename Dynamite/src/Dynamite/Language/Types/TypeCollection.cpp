#include "dypch.h"
#include "TypeCollection.hpp"

#include "Core/Logging.hpp"

#include "Utils/Checks.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// TypeCollection
	/////////////////////////////////////////////////////////////////
	void TypeCollection::Reset()
	{
		// Note: This function exists for the future
	}

	bool TypeCollection::ImplicitCastable(const Type& from, const Type& to)
	{
		// Checks
		{
			if (from == to)
				return true;

			if (from.IsPointer() && to.IsPointer())
				return true;

			if ((!from.IsPointer() && to.IsPointer()) || (from.IsPointer() && !to.IsPointer()))
				return false;

			if (from == TypeSpecifier::Void && to != TypeSpecifier::Void)
				return false;

			if (from.Information.Specifier == to.Information.Specifier)
				return true;
		}

		using namespace Pulse::Enum;
		switch (Fuse(from.Information.Specifier, to.Information.Specifier))
		{
		// Bool conversions
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::Int64, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Bool):			return true;
		case Fuse(TypeSpecifier::UInt64, TypeSpecifier::Bool):			return true;

		// Int to other Int conversions
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Int16):			return true;
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Int32):			return true;
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Int64):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Int32):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Int64):			return true;
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::Int64):			return true;

		// UInt to other UInt conversions
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::UInt16):			return true;
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::UInt32):			return true;
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::UInt64):			return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::UInt32):		return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::UInt64):		return true;
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::UInt64):		return true;

		// UInt to Int conversions
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int16):			return true;
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int32):			return true;
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int64):			return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Int32):			return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Int64):			return true;
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Int64):			return true;

		// Char conversions
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int8):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int16):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int32):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int64):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt8):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt16):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt32):			return true;
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt64):			return true;

		default:
			break;
		}

		return false;
	}

	bool TypeCollection::ExplicitCastable(const Type& from, const Type& to)
	{
		if (TypeCollection::ImplicitCastable(from, to))
			return true;

		using namespace Pulse::Enum;
		switch (Fuse(from.Information.Specifier, to.Information.Specifier))
		{
		// Int to UInt
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt16):			return true;
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt32):			return true;
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt64):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::UInt32):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::UInt64):			return true;
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::UInt64):			return true;
			
		// Same size UInt & Int conversions
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt8):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::UInt16):			return true;
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::UInt32):			return true;
		case Fuse(TypeSpecifier::Int64, TypeSpecifier::UInt64):			return true;
			
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int8):			return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Int16):			return true;
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Int32):			return true;
		case Fuse(TypeSpecifier::UInt64, TypeSpecifier::Int64):			return true;

		// Int to float
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Float32):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Float32):		return true;
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::Float32):		return true;

		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Float32):		return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Float32):		return true;
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Float32):		return true;

		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Float64):			return true;
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Float64):		return true;
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::Float64):		return true;
		case Fuse(TypeSpecifier::Int64, TypeSpecifier::Float64):		return true;

		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Float64):		return true;
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Float64):		return true;
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Float64):		return true;
		case Fuse(TypeSpecifier::UInt64, TypeSpecifier::Float64):		return true;

		// Float to int
		case Fuse(TypeSpecifier::Float32, TypeSpecifier::Int32):		return true;
		case Fuse(TypeSpecifier::Float32, TypeSpecifier::UInt32):		return true;
		case Fuse(TypeSpecifier::Float32, TypeSpecifier::Int64):		return true;
		case Fuse(TypeSpecifier::Float32, TypeSpecifier::UInt64):		return true;

		case Fuse(TypeSpecifier::Float64, TypeSpecifier::Int64):		return true;
		case Fuse(TypeSpecifier::Float64, TypeSpecifier::UInt64):		return true;

		default:
			break;
		}

		return false;
	}

	/////////////////////////////////////////////////////////////////
	// Main get functions
	/////////////////////////////////////////////////////////////////
	Type TypeCollection::GetTypeFromLiteral(LiteralType literalType, const std::string& value)
	{
		switch (literalType)
		{
		case LiteralType::BoolLiteral:
		{
			return Type({}, TypeSpecifier::Bool);
		}
		case LiteralType::IntegerLiteral:
		{
			bool isNegative = !value.empty() && value[0] == '-';

			if (isNegative)
			{
				int64_t intVal = std::stoll(value);

				if (intVal >= Pulse::Numeric::Min<int8_t>() && intVal <= Pulse::Numeric::Max<int8_t>())
					return Type({}, TypeSpecifier::Int8);
				else if (intVal >= Pulse::Numeric::Min<int16_t>() && intVal <= Pulse::Numeric::Max<int16_t>())
					return Type({}, TypeSpecifier::Int16);
				else if (intVal >= Pulse::Numeric::Min<int32_t>() && intVal <= Pulse::Numeric::Max<int32_t>())
					return Type({}, TypeSpecifier::Int32);
				else if (intVal >= Pulse::Numeric::Min<int64_t>() && intVal <= Pulse::Numeric::Max<int64_t>())
					return Type({}, TypeSpecifier::Int64);
				else
					DY_LOG_ERROR("Integer {0} exceeds max integers' type (Int64) size.", value);
			}
			else
			{
				uint64_t uintVal = std::stoull(value);

				if (uintVal <= Pulse::Numeric::Max<uint8_t>())
					return Type({}, TypeSpecifier::UInt8);
				else if (uintVal <= Pulse::Numeric::Max<uint16_t>())
					return Type({}, TypeSpecifier::UInt16);
				else if (uintVal <= Pulse::Numeric::Max<uint32_t>())
					return Type({}, TypeSpecifier::UInt32);
				else if (uintVal <= Pulse::Numeric::Max<uint64_t>())
					return Type({}, TypeSpecifier::UInt64);
				else
					DY_LOG_ERROR("Integer {0} exceeds max integers' type (UInt64) size.", value);
			}
			break;
		}
		case LiteralType::FloatLiteral:
		{
			double doubleVal = std::stod(value);

			if (doubleVal >= Pulse::Numeric::Min<float>() && doubleVal <= Pulse::Numeric::Max<float>())
				return Type({}, TypeSpecifier::Float32);
			if (doubleVal >= Pulse::Numeric::Min<double>() && doubleVal <= Pulse::Numeric::Max<double>())
				return Type({}, TypeSpecifier::Float64);
			else
				DY_LOG_ERROR("Float {0} exceeds max floats' type (Float64) size.", value);

			break;
		}
		case LiteralType::CharLiteral:
		{
			return Type({}, TypeSpecifier::Char);
		}
		case LiteralType::CharArrayLiteral:
		{
			return Type({}, TypeSpecifier::Char, { { TypeQualifier::Array, std::to_string(value.size() + 1) }}); // + 1, for the \0 character
		}

		default:
			break;
		}

		return {};
	}

	Type TypeCollection::GetBinExprResultType(const Type& lhs, OperationType operation, const Type& rhs)
	{
		if (operation == OperationType::Address || operation == OperationType::Dereference)
		{
			DY_LOG_ERROR("[Internal Compiler Error] Cannot retrieve the type of a binary operation with operation type '&' or '*' as Address or Dereference.");
			return {};
		}

		// Checks
		{
			if (lhs == rhs)
				return lhs.Clean();

			if (lhs.IsPointer() && rhs.IsPointer())
				return lhs.Clean();

			if ((lhs.IsIntegral() && rhs.IsFloat()) || (lhs.IsFloat() && rhs.IsIntegral()))
				return GetFloat(GetSize(GetLargest(lhs, rhs)));

			// TODO: Add more checks
		}

		return GetLargest(lhs, rhs);
	}

	bool TypeCollection::Exists(const std::string& name)
	{
		// Note: This function exists for the future
		return false;
	}

	size_t TypeCollection::GetSize(const Type& type)
	{
		if (type.IsPointer() || type.IsArray())
			return sizeof(void*);

		switch (type.Information.Specifier)
		{
		case TypeSpecifier::Void:			return 0;

		case TypeSpecifier::Bool:			return sizeof(bool);

		case TypeSpecifier::Int8:			return sizeof(int8_t);
		case TypeSpecifier::Int16:			return sizeof(int16_t);
		case TypeSpecifier::Int32:			return sizeof(int32_t);
		case TypeSpecifier::Int64:			return sizeof(int64_t);

		case TypeSpecifier::UInt8:			return sizeof(uint8_t);
		case TypeSpecifier::UInt16:			return sizeof(uint16_t);
		case TypeSpecifier::UInt32:			return sizeof(uint32_t);
		case TypeSpecifier::UInt64:			return sizeof(uint64_t);

		case TypeSpecifier::Float32:		return sizeof(float);
		case TypeSpecifier::Float64:		return sizeof(double);

		case TypeSpecifier::Char:			return sizeof(char);

		default:
			break;
		}

		return 0;
	}

	Type TypeCollection::GetLargest(const Type& lhs, const Type& rhs)
	{
		size_t lhsSize = GetSize(lhs);
		size_t rhsSize = GetSize(rhs);

		if (lhsSize == rhsSize)
			return lhs;

		if (lhsSize > rhsSize)
			return lhs;

		return rhs;
	}

	Type TypeCollection::GetInt(size_t size)
	{
		switch (size)
		{
		case sizeof(int8_t):			return Type(TypeSpecifier::Int8);
		case sizeof(int16_t):			return Type(TypeSpecifier::Int16);
		case sizeof(int32_t):			return Type(TypeSpecifier::Int32);
		case sizeof(int64_t):			return Type(TypeSpecifier::Int64);

		default:
			break;
		}

		return Type(TypeSpecifier::None);
	}

	Type TypeCollection::GetUInt(size_t size)
	{
		switch (size)
		{
		case sizeof(int8_t):			return Type(TypeSpecifier::UInt8);
		case sizeof(int16_t):			return Type(TypeSpecifier::UInt16);
		case sizeof(int32_t):			return Type(TypeSpecifier::UInt32);
		case sizeof(int64_t):			return Type(TypeSpecifier::UInt64);

		default:
			break;
		}

		return Type(TypeSpecifier::None);
	}

	Type TypeCollection::GetFloat(size_t size)
	{
		switch (size)
		{
		case sizeof(float):				return Type(TypeSpecifier::Float32);
		case sizeof(double):			return Type(TypeSpecifier::Float64);

		default:
			break;
		}

		return Type(TypeSpecifier::None);
	}

	/////////////////////////////////////////////////////////////////
	// Debug
	/////////////////////////////////////////////////////////////////
	std::string TypeCollection::ToString(const Type& type)
	{
		std::string str = {};

		for (const auto& [qualifier, value] : type.FrontQualifiers)
			str += TypeQualifierToString(qualifier, value) + ' ';

		if (type.Information.Specifier == TypeSpecifier::Identifier)
			str += type.Information.Value;
		else
			str += TypeSpecifierToString(type.Information.Specifier);

		for (const auto& [qualifier, value] : type.BackQualifiers)
			str += TypeQualifierToString(qualifier, value);

		return str;
	}

}
#include "dypch.h"
#include "TypeSystem.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// TypeSystem
	/////////////////////////////////////////////////////////////////
	void TypeSystem::Reset()
	{
		// Note: This function exists for the future
	}

	bool TypeSystem::Castable(const Type& from, const Type& to)
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

		// Int to UInt
		// /*
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
		// */

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

	bool TypeSystem::Cast(const Type& from, const Type& to, std::string& value)
	{
		if (from == to)
			return false;

		// Note: We don't perform any checks here since it should only cast if TypeSystem::Castable

		using namespace Pulse::Enum;
		switch (Fuse(from.Information.Specifier, to.Information.Specifier))
		{
			// Bool conversions
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::Int64, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Bool):
		case Fuse(TypeSpecifier::UInt64, TypeSpecifier::Bool):
		{
			uint64_t uint = std::stoull(value);
			bool b = static_cast<bool>(uint);
			value = std::to_string(b);
			
			return false;
		}

		// Smaller to larger Int & Uint conversions
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Int16):
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Int32):
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::Int64):
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Int32):
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::Int64):
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::Int64):

		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::UInt16):
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::UInt32):
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::UInt64):
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::UInt32):
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::UInt64):
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::UInt64):
		{
			return false;
		}

		// UInt to Int conversions
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int16):
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int32):
		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int64):
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Int32):
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Int64):
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Int64):
		{
			uint64_t uint = std::stoull(value);
			int64_t intVal = static_cast<int64_t>(uint);
			value = std::to_string(intVal);
			
			return false;
		}

		// Int to UInt conversions
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt16):
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt32):
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt64):
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::UInt32):
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::UInt64):
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::UInt64):
		{
			int64_t intVal = std::stoll(value);
			if (intVal < 0)
			{
				value = std::to_string(0); // Clamp to 0 for unsigned conversion
				return true;
			}

			value = std::to_string(static_cast<uint64_t>(intVal));
			
			return false;
		}

		// Same size UInt & Int conversions (with boundary checks)
		case Fuse(TypeSpecifier::Int8, TypeSpecifier::UInt8):
		case Fuse(TypeSpecifier::Int16, TypeSpecifier::UInt16):
		case Fuse(TypeSpecifier::Int32, TypeSpecifier::UInt32):
		case Fuse(TypeSpecifier::Int64, TypeSpecifier::UInt64):
		{
			int64_t intVal = std::stoll(value);
			if (intVal < 0)
			{
				value = std::to_string(0); // Clamp to 0 for unsigned conversion
				return true;
			}

			value = std::to_string(static_cast<uint64_t>(intVal));
			return false;
		}

		case Fuse(TypeSpecifier::UInt8, TypeSpecifier::Int8):
		case Fuse(TypeSpecifier::UInt16, TypeSpecifier::Int16):
		case Fuse(TypeSpecifier::UInt32, TypeSpecifier::Int32):
		case Fuse(TypeSpecifier::UInt64, TypeSpecifier::Int64):
		{
			uint64_t uintVal = std::stoull(value);
			if (uintVal > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
			{
				value =  std::to_string(std::numeric_limits<int64_t>::max()); // Clamp to max int value
				return true;
			}

			value = std::to_string(static_cast<int64_t>(uintVal));
			return false;
		}

		// Char conversions
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int8):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int16):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int32):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::Int64):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt8):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt16):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt32):
		case Fuse(TypeSpecifier::Char, TypeSpecifier::UInt64):
		{
			char c = value[0];
			int val = static_cast<int>(c);
			value = std::to_string(val);
			
			return false;
		}

		default:
			break;
		}

		DY_LOG_ERROR("Undefined cast from: {0} to {1}", TypeSystem::ToString(from), TypeSystem::ToString(to));
		return {};
	}

	Type TypeSystem::GetTypeFromLiteral(TokenType literalType, const std::string& value)
	{
		if (!Utils::IsEqual(literalType, GetAllTokenTypeLiterals()))
		{
			DY_LOG_WARN("Tried to get type from literal, but TokenType::{0} is not a literal.", Pulse::Enum::Name(literalType));
			return {};
		}

		switch (literalType)
		{
		case TokenType::BoolLiteral:
		{
			return Type({}, TypeSpecifier::Bool);
		}
		case TokenType::IntegerLiteral:
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
		case TokenType::FloatLiteral:
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
		case TokenType::CharLiteral:
		{
			return Type({}, TypeSpecifier::Char);
		}
		case TokenType::CharArrayLiteral:
		{
			return Type({}, TypeSpecifier::Char, { {TypeQualifier::Array} });
		}

		default:
			break;
		}

		return {};
	}

	Type TypeSystem::GetBinExprResultType(const Type& lhs, TokenType operation, const Type& rhs)
	{
		// Checks
		{
			if (lhs == rhs)
				return lhs.Clean();

			if (lhs.IsPointer() && rhs.IsPointer())
				return lhs.Clean();
		
			if ((IsIntegral(lhs) && IsFloat(rhs)) || (IsFloat(lhs) && IsIntegral(rhs)))
				return GetFloat(GetSize(GetLargest(lhs, rhs)));

			// TODO: Add more checks
		}

		return GetLargest(lhs, rhs);
	}

	std::string TypeSystem::ToString(const Type& type)
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

	bool TypeSystem::Exists(const std::string& name)
	{
		return false;
	}

	/////////////////////////////////////////////////////////////////
	// Check functions
	/////////////////////////////////////////////////////////////////
	bool TypeSystem::IsVoid(const Type& type)
	{
		return type.Information.Specifier == TypeSpecifier::Void;
	}

	bool TypeSystem::IsBool(const Type& type)
	{
		if (type.IsPointer())
			return false;

		if (type.Information.Specifier == TypeSpecifier::Bool)
		{
			return true;
		}

		return false;
	}

	bool TypeSystem::IsIntegral(const Type& type)
	{
		if (type.IsPointer())
			return false;

		if (type.Information.Specifier == TypeSpecifier::Int8 ||
			type.Information.Specifier == TypeSpecifier::Int16 ||
			type.Information.Specifier == TypeSpecifier::Int32 ||
			type.Information.Specifier == TypeSpecifier::Int64 ||

			type.Information.Specifier == TypeSpecifier::UInt8 ||
			type.Information.Specifier == TypeSpecifier::UInt16 ||
			type.Information.Specifier == TypeSpecifier::UInt32 ||
			type.Information.Specifier == TypeSpecifier::UInt64)
		{
			return true;
		}

		return false;
	}

	bool TypeSystem::IsFloat(const Type& type)
	{
		if (type.IsPointer())
			return false;

		if (type.Information.Specifier == TypeSpecifier::Float32 ||
			type.Information.Specifier == TypeSpecifier::Float64)
		{
			return true;
		}

		return false;
	}

	bool TypeSystem::IsChar(const Type& type)
	{
		if (type.IsPointer())
			return false;

		if (type.Information.Specifier == TypeSpecifier::Char)
		{
			return true;
		}

		return false;
	}

	size_t TypeSystem::GetSize(const Type& type)
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

	Type TypeSystem::GetLargest(const Type& lhs, const Type& rhs)
	{
		size_t lhsSize = GetSize(lhs);
		size_t rhsSize = GetSize(rhs);

		if (lhsSize == rhsSize)
			return lhs;
		
		if (lhsSize > rhsSize)
			return lhs;

		return rhs;
	}

	Type TypeSystem::GetInt(size_t size)
	{
		switch (size)
		{
		case sizeof(int8_t):				return Type(TypeSpecifier::Int8);
		case sizeof(int16_t):				return Type(TypeSpecifier::Int16);
		case sizeof(int32_t):				return Type(TypeSpecifier::Int32);
		case sizeof(int64_t):				return Type(TypeSpecifier::Int64);

		default:
			break;
		}

		return Type(TypeSpecifier::None);
	}

	Type TypeSystem::GetUInt(size_t size)
	{
		switch (size)
		{
		case sizeof(int8_t):				return Type(TypeSpecifier::UInt8);
		case sizeof(int16_t):				return Type(TypeSpecifier::UInt16);
		case sizeof(int32_t):				return Type(TypeSpecifier::UInt32);
		case sizeof(int64_t):				return Type(TypeSpecifier::UInt64);

		default:
			break;
		}

		return Type(TypeSpecifier::None);
	}

	Type TypeSystem::GetFloat(size_t size)
	{
		switch (size)
		{
		case sizeof(float):					return Type(TypeSpecifier::Float32);
		case sizeof(double):				return Type(TypeSpecifier::Float64);

		default:
			break;
		}

		return Type(TypeSpecifier::None);
	}

}
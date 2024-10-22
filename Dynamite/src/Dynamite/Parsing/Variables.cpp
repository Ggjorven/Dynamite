#include "dypch.h"
#include "Variables.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	ValueType::ValueType(BaseType base, ValueType* next)
		: Base(base), Next(next)
	{
	}

	ValueType::~ValueType()
	{
		//if (Next)
		//	delete Next;
	}

	bool ValueType::operator == (BaseType otherBase) const
	{
		return this->Base == otherBase;
	}

	bool ValueType::operator != (BaseType otherBase) const
	{
		return !(this->Base == otherBase);
	}

	bool ValueType::operator == (const ValueType& other) const
	{
		bool result = (this->Base == other.Base);
		
		if (this->Next && other.Next)
			result |= (*(this->Next) == *other.Next);
		
		return result;
	}

	bool ValueType::operator != (const ValueType& other) const
	{
		return !(*this == other);
	}

	std::string ValueTypeToStr(const ValueType& type)
	{
		switch (type.Base)
		{
		case BaseType::Void:		return "void";

		case BaseType::Bool:		return "bool";

		case BaseType::Int8:		return "i8";
		case BaseType::Int16:		return "i16";
		case BaseType::Int32:		return "i32";
		case BaseType::Int64:		return "i64";

		case BaseType::UInt8:		return "u8";
		case BaseType::UInt16:		return "u16";
		case BaseType::UInt32:		return "u32";
		case BaseType::UInt64:		return "u64";

		case BaseType::Float32:		return "f32";
		case BaseType::Float64:		return "f64";

		case BaseType::Char:		return "char";
		case BaseType::String:		return "str";

		case BaseType::Pointer:		return ValueTypeToStr(*type.Next) + '*';

		default:
			break;
		}

		DY_LOG_ERROR("ValueType::{0}, Name has not been defined.", Pulse::Enum::Name(type.Base));
		return "UNDEFINED ValueType";
	}

	size_t ValueTypeSize(const ValueType& type)
	{
		switch (type.Base)
		{
		case BaseType::Void:		return 0;

		case BaseType::Bool:		return sizeof(bool);

		case BaseType::Int8:		return sizeof(int8_t);
		case BaseType::Int16:		return sizeof(int16_t);
		case BaseType::Int32:		return sizeof(int32_t);
		case BaseType::Int64:		return sizeof(int64_t);

		case BaseType::UInt8:		return sizeof(uint8_t);
		case BaseType::UInt16:		return sizeof(uint16_t);
		case BaseType::UInt32:		return sizeof(uint32_t);
		case BaseType::UInt64:		return sizeof(uint64_t);

		case BaseType::Float32:		return sizeof(float);
		case BaseType::Float64:		return sizeof(double);

		case BaseType::Char:		return sizeof(char);
		//case ValueType::String:		return sizeof(ValueTypeToCType<ValueType::String>);

		case BaseType::Pointer:		return sizeof(void*);

		default:
			break;
		}

		DY_LOG_ERROR("ValueType::{0}, Size has not been defined.", Pulse::Enum::Name(type.Base));
		return 0;
	}

	bool ValueTypeCastable(const ValueType& from, const ValueType& to)
	{
		if (from.Base == to.Base)
			return true;

		if (from.Base == BaseType::Void || to.Base == BaseType::Void)
			return false;

		using namespace Pulse::Enum;
		switch (Fuse(from.Base, to.Base))
		{
		// Bool conversions
		case Fuse(BaseType::Int8, BaseType::Bool):			return true;
		case Fuse(BaseType::Int16, BaseType::Bool):			return true;
		case Fuse(BaseType::Int32, BaseType::Bool):			return true;
		case Fuse(BaseType::Int64, BaseType::Bool):			return true;
		case Fuse(BaseType::UInt8, BaseType::Bool):			return true;
		case Fuse(BaseType::UInt16, BaseType::Bool):		return true;
		case Fuse(BaseType::UInt32, BaseType::Bool):		return true;
		case Fuse(BaseType::UInt64, BaseType::Bool):		return true;

		// Int to other Int conversions
		case Fuse(BaseType::Int8, BaseType::Int16):			return true;
		case Fuse(BaseType::Int8, BaseType::Int32):			return true;
		case Fuse(BaseType::Int8, BaseType::Int64):			return true;
		case Fuse(BaseType::Int16, BaseType::Int32):		return true;
		case Fuse(BaseType::Int16, BaseType::Int64):		return true;
		case Fuse(BaseType::Int32, BaseType::Int64):		return true;

		// UInt to other UInt conversions
		case Fuse(BaseType::UInt8, BaseType::UInt16):		return true;
		case Fuse(BaseType::UInt8, BaseType::UInt32):		return true;
		case Fuse(BaseType::UInt8, BaseType::UInt64):		return true;
		case Fuse(BaseType::UInt16, BaseType::UInt32):		return true;
		case Fuse(BaseType::UInt16, BaseType::UInt64):		return true;
		case Fuse(BaseType::UInt32, BaseType::UInt64):		return true;

		// UInt to Int conversions
		case Fuse(BaseType::UInt8, BaseType::Int16):		return true;
		case Fuse(BaseType::UInt8, BaseType::Int32):		return true;
		case Fuse(BaseType::UInt8, BaseType::Int64):		return true;
		case Fuse(BaseType::UInt16, BaseType::Int32):		return true;
		case Fuse(BaseType::UInt16, BaseType::Int64):		return true;
		case Fuse(BaseType::UInt32, BaseType::Int64):		return true;

		// Int to UInt
		// /*
		case Fuse(BaseType::Int8, BaseType::UInt16):		return true;
		case Fuse(BaseType::Int8, BaseType::UInt32):		return true;
		case Fuse(BaseType::Int8, BaseType::UInt64):		return true;
		case Fuse(BaseType::Int16, BaseType::UInt32):		return true;
		case Fuse(BaseType::Int16, BaseType::UInt64):		return true;
		case Fuse(BaseType::Int32, BaseType::UInt64):		return true;

		// Same size UInt & Int conversions
		case Fuse(BaseType::Int8, BaseType::UInt8):			return true;
		case Fuse(BaseType::Int16, BaseType::UInt16):		return true;
		case Fuse(BaseType::Int32, BaseType::UInt32):		return true;
		case Fuse(BaseType::Int64, BaseType::UInt64):		return true;

		case Fuse(BaseType::UInt8, BaseType::Int8):			return true;
		case Fuse(BaseType::UInt16, BaseType::Int16):		return true;
		case Fuse(BaseType::UInt32, BaseType::Int32):		return true;
		case Fuse(BaseType::UInt64, BaseType::Int64):		return true;
		// */

		// Char conversions
		case Fuse(BaseType::Char, BaseType::Int8):			return true;
		case Fuse(BaseType::Char, BaseType::Int16):			return true;
		case Fuse(BaseType::Char, BaseType::Int32):			return true;
		case Fuse(BaseType::Char, BaseType::Int64):			return true;
		case Fuse(BaseType::Char, BaseType::UInt8):			return true;
		case Fuse(BaseType::Char, BaseType::UInt16):		return true;
		case Fuse(BaseType::Char, BaseType::UInt32):		return true;
		case Fuse(BaseType::Char, BaseType::UInt64):		return true;

		// TODO: Add the rest

		// TODO: Add pointers

		default:
			break;
		}

		return false;
	}

	std::string ValueTypeCast(const ValueType& from, const ValueType& to, const std::string& value, bool* dataLostPtr)
	{
		if (from == to)
			return value;

		using namespace Pulse::Enum;
		switch (Fuse(from.Base, to.Base))
		{
		// Bool conversions
		case Fuse(BaseType::Int8, BaseType::Bool):
		case Fuse(BaseType::Int16, BaseType::Bool):		
		case Fuse(BaseType::Int32, BaseType::Bool):		
		case Fuse(BaseType::Int64, BaseType::Bool):		
		case Fuse(BaseType::UInt8, BaseType::Bool):		
		case Fuse(BaseType::UInt16, BaseType::Bool):		
		case Fuse(BaseType::UInt32, BaseType::Bool):		
		case Fuse(BaseType::UInt64, BaseType::Bool):		
		{
			uint64_t uint = std::stoull(value);
			bool b = static_cast<bool>(uint);
			return std::to_string(b);
		}

		// Smaller to larger Int & Uint conversions
		case Fuse(BaseType::Int8, BaseType::Int16):	
		case Fuse(BaseType::Int8, BaseType::Int32):	
		case Fuse(BaseType::Int8, BaseType::Int64):	
		case Fuse(BaseType::Int16, BaseType::Int32):	
		case Fuse(BaseType::Int16, BaseType::Int64):	
		case Fuse(BaseType::Int32, BaseType::Int64):	

		case Fuse(BaseType::UInt8, BaseType::UInt16):	
		case Fuse(BaseType::UInt8, BaseType::UInt32):	
		case Fuse(BaseType::UInt8, BaseType::UInt64):	
		case Fuse(BaseType::UInt16, BaseType::UInt32):
		case Fuse(BaseType::UInt16, BaseType::UInt64):
		case Fuse(BaseType::UInt32, BaseType::UInt64):
		{
			return value;
		}

		// UInt to Int conversions
		case Fuse(BaseType::UInt8, BaseType::Int16):
		case Fuse(BaseType::UInt8, BaseType::Int32):
		case Fuse(BaseType::UInt8, BaseType::Int64):
		case Fuse(BaseType::UInt16, BaseType::Int32):
		case Fuse(BaseType::UInt16, BaseType::Int64):
		case Fuse(BaseType::UInt32, BaseType::Int64):
		{
			uint64_t uint = std::stoull(value);
			int64_t intVal = static_cast<int64_t>(uint);
			return std::to_string(intVal);
		}

		// Int to UInt conversions
		case Fuse(BaseType::Int8, BaseType::UInt16):
		case Fuse(BaseType::Int8, BaseType::UInt32):
		case Fuse(BaseType::Int8, BaseType::UInt64):
		case Fuse(BaseType::Int16, BaseType::UInt32):
		case Fuse(BaseType::Int16, BaseType::UInt64):
		case Fuse(BaseType::Int32, BaseType::UInt64):
		{
			int64_t intVal = std::stoll(value);
			if (intVal < 0)
			{
				if (dataLostPtr) *dataLostPtr = true;
				return std::to_string(0); // Clamp to zero for unsigned types
			}

			return std::to_string(static_cast<uint64_t>(intVal));
		}

		// Same size UInt & Int conversions (with boundary checks)
		case Fuse(BaseType::Int8, BaseType::UInt8):
		case Fuse(BaseType::Int16, BaseType::UInt16):
		case Fuse(BaseType::Int32, BaseType::UInt32):
		case Fuse(BaseType::Int64, BaseType::UInt64):
		{
			int64_t intVal = std::stoll(value);
			if (intVal < 0)
			{
				if (dataLostPtr) *dataLostPtr = true;
				return std::to_string(0); // Clamp to 0 for unsigned conversion
			}

			return std::to_string(static_cast<uint64_t>(intVal));
		}

		case Fuse(BaseType::UInt8, BaseType::Int8):
		case Fuse(BaseType::UInt16, BaseType::Int16):
		case Fuse(BaseType::UInt32, BaseType::Int32):
		case Fuse(BaseType::UInt64, BaseType::Int64):
		{
			uint64_t uintVal = std::stoull(value);
			if (uintVal > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
			{
				if (dataLostPtr) *dataLostPtr = true;
				return std::to_string(std::numeric_limits<int64_t>::max()); // Clamp to max int value
			}

			return std::to_string(static_cast<int64_t>(uintVal));
		}

		// Char conversions
		case Fuse(BaseType::Char, BaseType::Int8):
		case Fuse(BaseType::Char, BaseType::Int16):	
		case Fuse(BaseType::Char, BaseType::Int32):	
		case Fuse(BaseType::Char, BaseType::Int64):	
		case Fuse(BaseType::Char, BaseType::UInt8):	
		case Fuse(BaseType::Char, BaseType::UInt16):	
		case Fuse(BaseType::Char, BaseType::UInt32):	
		case Fuse(BaseType::Char, BaseType::UInt64):	
		{
			char c = value[0];
			int val = static_cast<int>(c);
			return std::to_string(val);
		}

		// TODO: Pointers

		default:
			break;
		}

		DY_LOG_ERROR("Undefined cast from: {0} to {1}", Pulse::Enum::Name(from.Base), Pulse::Enum::Name(to.Base));
		return {};
	}

	BaseType GetBaseType(TokenType literalType, const std::string& value)
	{
		BaseType type = BaseType::None;

		switch (literalType)
		{
		case TokenType::BoolLiteral:
		{
			type = BaseType::Bool;
			break;
		}
		case TokenType::IntegerLiteral:
		{
			bool isNegative = !value.empty() && value[0] == '-';

			if (isNegative)
			{
				int64_t intVal = std::stoll(value);

				if (intVal >= Pulse::Numeric::Min<int8_t>() && intVal <= Pulse::Numeric::Max<int8_t>())
					type = BaseType::Int8;
				else if (intVal >= Pulse::Numeric::Min<int16_t>() && intVal <= Pulse::Numeric::Max<int16_t>())
					type = BaseType::Int16;
				else if (intVal >= Pulse::Numeric::Min<int32_t>() && intVal <= Pulse::Numeric::Max<int32_t>())
					type = BaseType::Int32;
				else if (intVal >= Pulse::Numeric::Min<int64_t>() && intVal <= Pulse::Numeric::Max<int64_t>())
					type = BaseType::Int64;
				else
					DY_LOG_ERROR("Integer {0} exceeds max integers' type (Int64) size.", value);
			}
			else
			{
				uint64_t uintVal = std::stoull(value);

				if (uintVal <= Pulse::Numeric::Max<uint8_t>())
					type = BaseType::UInt8;
				else if (uintVal <= Pulse::Numeric::Max<uint16_t>())
					type = BaseType::UInt16;
				else if (uintVal <= Pulse::Numeric::Max<uint32_t>())
					type = BaseType::UInt32;
				else if (uintVal <= Pulse::Numeric::Max<uint64_t>())
					type = BaseType::UInt64;
				else
					DY_LOG_ERROR("Integer {0} exceeds max integers' type (UInt64) size.", value);
			}
			break;
		}
		case TokenType::FloatLiteral:
		{
			double doubleVal = std::stod(value);

			if (doubleVal >= Pulse::Numeric::Min<float>() && doubleVal <= Pulse::Numeric::Max<float>())
				type = BaseType::Float32;
			if (doubleVal >= Pulse::Numeric::Min<double>() && doubleVal <= Pulse::Numeric::Max<double>())
				type = BaseType::Float32;
			else
				DY_LOG_ERROR("Float {0} exceeds max floats' type (Float64) size.", value);

			break;
		}
		case TokenType::CharLiteral:
		{
			type = BaseType::Char;
			break;
		}
		case TokenType::StringLiteral:
		{
			type = BaseType::String;
			break;
		}

		default:
			break;
		}

		return type;
	}

}
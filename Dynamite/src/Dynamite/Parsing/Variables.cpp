#include "dypch.h"
#include "Variables.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	std::string ValueTypeToASM(ValueType type)
	{
		switch (type)
		{
		case ValueType::Int8:
		case ValueType::UInt8:
			return "BYTE";

		case ValueType::Int16:
		case ValueType::UInt16:
			return "WORD";

		case ValueType::Int32:     
		case ValueType::UInt32:     
			return "DWORD";

		case ValueType::Int64:     
		case ValueType::UInt64:     
			return "QWORD";

		// TODO: Other types

		default:
			break;
		}

		DY_LOG_ERROR("ValueType::{0}, ASM type has not been defined.", Pulse::Enum::Name(type));
		return "Undefined ASM Type";
	}

	std::string ValueTypeToStr(ValueType type)
	{
		switch (type)
		{
		case ValueType::Bool:		return "bool";

		case ValueType::Int8:		return "i8";
		case ValueType::Int16:		return "i16";
		case ValueType::Int32:		return "i32";
		case ValueType::Int64:		return "i64";

		case ValueType::UInt8:		return "u8";
		case ValueType::UInt16:		return "u16";
		case ValueType::UInt32:		return "u32";
		case ValueType::UInt64:		return "u64";

		case ValueType::Float32:	return "f32";
		case ValueType::Float64:	return "f64";

		case ValueType::Char:		return "char";
		case ValueType::String:		return "str";

		default:
			break;
		}

		return "Unnamed ValueType";
	}

	bool ValueTypeCastable(ValueType from, ValueType to)
	{
		if (from == to)
			return true;

		using namespace Pulse::Enum;
		switch (Fuse(from, to))
		{
		// Bool conversions
		case Fuse(ValueType::Int8, ValueType::Bool):		return true;
		case Fuse(ValueType::Int16, ValueType::Bool):		return true;
		case Fuse(ValueType::Int32, ValueType::Bool):		return true;
		case Fuse(ValueType::Int64, ValueType::Bool):		return true;
		case Fuse(ValueType::UInt8, ValueType::Bool):		return true;
		case Fuse(ValueType::UInt16, ValueType::Bool):		return true;
		case Fuse(ValueType::UInt32, ValueType::Bool):		return true;
		case Fuse(ValueType::UInt64, ValueType::Bool):		return true;

		// Int to other Int conversions
		case Fuse(ValueType::Int8, ValueType::Int16):		return true;
		case Fuse(ValueType::Int8, ValueType::Int32):		return true;
		case Fuse(ValueType::Int8, ValueType::Int64):		return true;
		case Fuse(ValueType::Int16, ValueType::Int32):		return true;
		case Fuse(ValueType::Int16, ValueType::Int64):		return true;
		case Fuse(ValueType::Int32, ValueType::Int64):		return true;

		// UInt to other UInt conversions
		case Fuse(ValueType::UInt8, ValueType::UInt16):		return true;
		case Fuse(ValueType::UInt8, ValueType::UInt32):		return true;
		case Fuse(ValueType::UInt8, ValueType::UInt64):		return true;
		case Fuse(ValueType::UInt16, ValueType::UInt32):	return true;
		case Fuse(ValueType::UInt16, ValueType::UInt64):	return true;
		case Fuse(ValueType::UInt32, ValueType::UInt64):	return true;

		// UInt to Int conversions
		case Fuse(ValueType::UInt8, ValueType::Int16):		return true;
		case Fuse(ValueType::UInt8, ValueType::Int32):		return true;
		case Fuse(ValueType::UInt8, ValueType::Int64):		return true;
		case Fuse(ValueType::UInt16, ValueType::Int32):		return true;
		case Fuse(ValueType::UInt16, ValueType::Int64):		return true;
		case Fuse(ValueType::UInt32, ValueType::Int64):		return true;

		// Int to UInt
		// /*
		case Fuse(ValueType::Int8, ValueType::UInt16):		return true;
		case Fuse(ValueType::Int8, ValueType::UInt32):		return true;
		case Fuse(ValueType::Int8, ValueType::UInt64):		return true;
		case Fuse(ValueType::Int16, ValueType::UInt32):		return true;
		case Fuse(ValueType::Int16, ValueType::UInt64):		return true;
		case Fuse(ValueType::Int32, ValueType::UInt64):		return true;

		// Same size UInt & Int conversions
		case Fuse(ValueType::Int8, ValueType::UInt8):		return true;
		case Fuse(ValueType::Int16, ValueType::UInt16):		return true;
		case Fuse(ValueType::Int32, ValueType::UInt32):		return true;
		case Fuse(ValueType::Int64, ValueType::UInt64):		return true;

		case Fuse(ValueType::UInt8, ValueType::Int8):		return true;
		case Fuse(ValueType::UInt16, ValueType::Int16):		return true;
		case Fuse(ValueType::UInt32, ValueType::Int32):		return true;
		case Fuse(ValueType::UInt64, ValueType::Int64):		return true;
		// */

		// Char conversions
		case Fuse(ValueType::Char, ValueType::Int8):		return true;
		case Fuse(ValueType::Char, ValueType::Int16):		return true;
		case Fuse(ValueType::Char, ValueType::Int32):		return true;
		case Fuse(ValueType::Char, ValueType::Int64):		return true;
		case Fuse(ValueType::Char, ValueType::UInt8):		return true;
		case Fuse(ValueType::Char, ValueType::UInt16):		return true;
		case Fuse(ValueType::Char, ValueType::UInt32):		return true;
		case Fuse(ValueType::Char, ValueType::UInt64):		return true;

		// TODO: Add the rest

		default:
			break;
		}

		return false;
	}

	std::string ValueTypeCast(ValueType from, ValueType to, const std::string& value, bool* dataLostPtr)
	{
		if (from == to)
			return value;

		using namespace Pulse::Enum;
		switch (Fuse(from, to))
		{
		// Bool conversions
		case Fuse(ValueType::Int8, ValueType::Bool):		
		case Fuse(ValueType::Int16, ValueType::Bool):		
		case Fuse(ValueType::Int32, ValueType::Bool):		
		case Fuse(ValueType::Int64, ValueType::Bool):		
		case Fuse(ValueType::UInt8, ValueType::Bool):		
		case Fuse(ValueType::UInt16, ValueType::Bool):		
		case Fuse(ValueType::UInt32, ValueType::Bool):		
		case Fuse(ValueType::UInt64, ValueType::Bool):		
		{
			uint64_t uint = std::stoull(value);
			bool b = static_cast<bool>(uint);
			return std::to_string(b);
		}

		// Smaller to larger Int & Uint conversions
		case Fuse(ValueType::Int8, ValueType::Int16):	
		case Fuse(ValueType::Int8, ValueType::Int32):	
		case Fuse(ValueType::Int8, ValueType::Int64):	
		case Fuse(ValueType::Int16, ValueType::Int32):	
		case Fuse(ValueType::Int16, ValueType::Int64):	
		case Fuse(ValueType::Int32, ValueType::Int64):	

		case Fuse(ValueType::UInt8, ValueType::UInt16):	
		case Fuse(ValueType::UInt8, ValueType::UInt32):	
		case Fuse(ValueType::UInt8, ValueType::UInt64):	
		case Fuse(ValueType::UInt16, ValueType::UInt32):
		case Fuse(ValueType::UInt16, ValueType::UInt64):
		case Fuse(ValueType::UInt32, ValueType::UInt64):
		{
			return value;
		}

		// UInt to Int conversions
		case Fuse(ValueType::UInt8, ValueType::Int16):
		case Fuse(ValueType::UInt8, ValueType::Int32):
		case Fuse(ValueType::UInt8, ValueType::Int64):
		case Fuse(ValueType::UInt16, ValueType::Int32):
		case Fuse(ValueType::UInt16, ValueType::Int64):
		case Fuse(ValueType::UInt32, ValueType::Int64):
		{
			uint64_t uint = std::stoull(value);
			int64_t intVal = static_cast<int64_t>(uint);
			return std::to_string(intVal);
		}

		// Int to UInt conversions
		case Fuse(ValueType::Int8, ValueType::UInt16):
		case Fuse(ValueType::Int8, ValueType::UInt32):
		case Fuse(ValueType::Int8, ValueType::UInt64):
		case Fuse(ValueType::Int16, ValueType::UInt32):
		case Fuse(ValueType::Int16, ValueType::UInt64):
		case Fuse(ValueType::Int32, ValueType::UInt64):
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
		case Fuse(ValueType::Int8, ValueType::UInt8):
		case Fuse(ValueType::Int16, ValueType::UInt16):
		case Fuse(ValueType::Int32, ValueType::UInt32):
		case Fuse(ValueType::Int64, ValueType::UInt64):
		{
			int64_t intVal = std::stoll(value);
			if (intVal < 0)
			{
				if (dataLostPtr) *dataLostPtr = true;
				return std::to_string(0); // Clamp to 0 for unsigned conversion
			}

			return std::to_string(static_cast<uint64_t>(intVal));
		}

		case Fuse(ValueType::UInt8, ValueType::Int8):
		case Fuse(ValueType::UInt16, ValueType::Int16):
		case Fuse(ValueType::UInt32, ValueType::Int32):
		case Fuse(ValueType::UInt64, ValueType::Int64):
		{
			uint64_t uintVal = std::stoull(value);
			if (uintVal > std::numeric_limits<int64_t>::max())
			{
				if (dataLostPtr) *dataLostPtr = true;
				return std::to_string(std::numeric_limits<int64_t>::max()); // Clamp to max int value
			}

			return std::to_string(static_cast<int64_t>(uintVal));
		}

		// Char conversions
		case Fuse(ValueType::Char, ValueType::Int8):
		case Fuse(ValueType::Char, ValueType::Int16):	
		case Fuse(ValueType::Char, ValueType::Int32):	
		case Fuse(ValueType::Char, ValueType::Int64):	
		case Fuse(ValueType::Char, ValueType::UInt8):	
		case Fuse(ValueType::Char, ValueType::UInt16):	
		case Fuse(ValueType::Char, ValueType::UInt32):	
		case Fuse(ValueType::Char, ValueType::UInt64):	
		{
			char c = value[0];
			int val = static_cast<int>(c);
			return std::to_string(val);
		}

		default:
			break;
		}

		DY_LOG_ERROR("Undefined cast from: {0} to {1}", Pulse::Enum::Name(from), Pulse::Enum::Name(to));
		return {};
	}


}
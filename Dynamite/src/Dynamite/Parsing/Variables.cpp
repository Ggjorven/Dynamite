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
		//case ValueType::Int8:     return "BYTE";
		//case ValueType::Int16:     return "WORD";
		//case ValueType::Int32:     return "DWORD";
		case ValueType::Int64:     return "QWORD";

		default:
			break;
		}

		DY_LOG_ERROR("ValueType::{0}, ASM type has not been defined.", Pulse::Enum::Name(type));
		return "Undefined ASM Type";
	}

}
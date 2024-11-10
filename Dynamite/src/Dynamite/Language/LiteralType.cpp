#include "dypch.h"
#include "LiteralType.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Helper methods
	/////////////////////////////////////////////////////////////////
	LiteralType GetLiteralTypeFromType(const Type& type)
	{
		switch (type.Information.Specifier)
		{
		case TypeSpecifier::Bool:
			return LiteralType::BoolLiteral;

		case TypeSpecifier::Int8:
		case TypeSpecifier::Int16:
		case TypeSpecifier::Int32:
		case TypeSpecifier::Int64:
		case TypeSpecifier::UInt8:
		case TypeSpecifier::UInt16:
		case TypeSpecifier::UInt32:
		case TypeSpecifier::UInt64:
			return LiteralType::IntegerLiteral;
		
		case TypeSpecifier::Float32:
		case TypeSpecifier::Float64:
			return LiteralType::FloatLiteral;
		
		case TypeSpecifier::Char:
		{
			if (type.IsArray() || type.IsPointer())
				return LiteralType::CharArrayLiteral;
			
			return LiteralType::CharLiteral;
		}

		default:
			break;
		}

		DY_LOG_ERROR("[Internal Compiler Error] - Tried to retrieve LiteralType from Type with invalid type.");
		return LiteralType::None;
	}

}
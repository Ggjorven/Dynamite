#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Operation
	/////////////////////////////////////////////////////////////////
	enum class LiteralType : uint8_t
	{
		None = 0,

		BoolLiteral,

		IntegerLiteral,

		FloatLiteral,

		CharLiteral,
		CharArrayLiteral,
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	LiteralType GetLiteralTypeFromType(const Type& type);

}
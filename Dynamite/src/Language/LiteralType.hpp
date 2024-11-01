#pragma once

#include "Core/Core.hpp"

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

}
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// AccessSpecifier
	/////////////////////////////////////////////////////////////////
	enum class AccessSpecifier : uint8_t
	{
		None = 0,

		Public,
		Private
	};

}
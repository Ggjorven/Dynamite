#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <string>

namespace Dynamite::Utils
{

	// Str utilities
	std::string StrTimes(const std::string& str, size_t times);
	std::string StrLower(const std::string& val);
	std::string EmplaceAfterIndentation(const std::string& val, const std::string& place);
	std::string RemoveFrontIndentation(const std::string& val);

}
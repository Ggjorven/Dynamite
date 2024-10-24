#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>

namespace Dynamite::Utils
{

	std::string StrTimes(const std::string& str, size_t times);
	std::string& StrLower(std::string& val);
	std::string& EmplaceAfterIndentation(std::string& val, const std::string& place);
	std::string& RemoveFrontIndentation(std::string& val);

}
#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <string>

namespace Dynamite::Utils
{

	std::string& StrLower(std::string& val);
	std::string& EmplaceAfterIndentation(std::string& val, const std::string& place);
	std::string& RemoveFrontIndentation(std::string& val, const std::string& place);

}
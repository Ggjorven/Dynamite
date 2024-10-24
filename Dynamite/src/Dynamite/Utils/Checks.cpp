#include "dypch.h"
#include "Checks.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite::Utils
{

	bool IsAlpha(char c, const std::vector<char>& allowed)
	{
		return (std::isalpha(c) || IsEqual(c, allowed));
	}

	bool IsNumeric(char c, const std::vector<char>& allowed)
	{
		return (std::isdigit(c) || IsEqual(c, allowed));
	}

	bool IsAlphaNumeric(char c, const std::vector<char>& allowed)
	{
		return (IsAlpha(c, allowed) || IsNumeric(c, allowed));
	}

	bool IsSpace(char c, const std::vector<char>& allowed)
	{
		return (std::isspace(c) || IsEqual(c, allowed));
	}

}
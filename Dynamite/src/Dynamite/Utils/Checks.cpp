#include "dypch.h"
#include "Checks.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite::Utils
{

	bool IsAlpha(char c, const std::initializer_list<char>& allowed)
	{
		if (std::isalpha(c))
			return true;

		for (const auto& val : allowed)
		{
			if (c == val)
				return true;
		}

		return false;
	}

	bool IsNumeric(char c, const std::initializer_list<char>& allowed)
	{
		if (std::isdigit(c))
			return true;

		for (const auto& val : allowed)
		{
			if (c == val)
				return true;
		}

		return false;
	}

	bool IsAlphaNumeric(char c, const std::initializer_list<char>& allowed)
	{
		return (IsAlpha(c, allowed) || IsNumeric(c, allowed));
	}

	bool IsSpace(char c, const std::initializer_list<char>& allowed)
	{
		if (std::isspace(c))
			return true;

		for (const auto& val : allowed)
		{
			if (c == val)
				return true;
		}

		return false;
	}

}
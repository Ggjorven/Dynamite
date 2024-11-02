#include "dypch.h"
#include "Utils.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite::Utils
{

	std::string StrTimes(const std::string& str, size_t times)
	{
		std::string result = {};

		for (size_t i = 0; i < times; i++)
			result += str;

		return result;
	}

	std::string StrLower(const std::string& val)
	{
		std::string newStr = val;

		for (char& c : newStr)
			c = static_cast<char>(std::tolower(static_cast<int>(c)));

		return newStr;
	}

	std::string EmplaceAfterIndentation(const std::string& val, const std::string& place)
	{
		std::string newStr = val;

		std::size_t pos = newStr.find_first_not_of(" \t");

		// If no non-whitespace character is found, insert after all characters
		if (pos == std::string::npos)
			pos = newStr.size();

		// Insert the 'place' string after the indentation
		newStr.insert(pos, place);

		return newStr;
	}

    std::string RemoveFrontIndentation(const std::string& val)
    {
		std::string newStr = val;

		// Use std::find_if to find the first non-whitespace character
		newStr.erase(newStr.begin(), std::find_if(newStr.begin(), newStr.end(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}));

		return newStr;
    }

}
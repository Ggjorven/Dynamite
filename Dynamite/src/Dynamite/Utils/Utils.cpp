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

	std::string& StrLower(std::string& val)
	{
		for (char& c : val)
			c = std::tolower(c);

		return val;
	}

	std::string& EmplaceAfterIndentation(std::string& val, const std::string& place)
	{
		std::size_t pos = val.find_first_not_of(" \t");

		// If no non-whitespace character is found, insert after all characters
		if (pos == std::string::npos)
			pos = val.size();

		// Insert the 'place' string after the indentation
		val.insert(pos, place);

		return val;
	}

    std::string& RemoveFrontIndentation(std::string& val)
    {
		// Use std::find_if to find the first non-whitespace character
		val.erase(val.begin(), std::find_if(val.begin(), val.end(), [](unsigned char ch) 
		{
			return !std::isspace(ch);
		}));

		return val;
    }

}
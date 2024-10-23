#include "dypch.h"
#include "Utils.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite::Utils
{
	
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

    std::string& RemoveFrontIndentation(std::string& val, const std::string& place)
    {
        std::size_t pos = val.find_first_not_of(" \t");

        // If all characters are whitespace, clear the string
        if (pos == std::string::npos)
        {
            val.clear();
            return val;
        }

        if (val.compare(pos, place.size(), place) == 0)
            val.erase(0, pos + place.size());
        else
            val.erase(0, pos);

        return val;
    }


}
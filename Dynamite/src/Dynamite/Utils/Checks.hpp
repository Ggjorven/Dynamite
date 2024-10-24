#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <vector>

namespace Dynamite::Utils
{

    // Text based utils
    bool IsAlpha(char c, const std::vector<char>& allowed = { });
    bool IsNumeric(char c, const std::vector<char>& allowed = { });
    bool IsAlphaNumeric(char c, const std::vector<char>& allowed = { });
    bool IsSpace(char c, const std::vector<char>& allowed = { });
    
    // Other
    template<typename T>
    bool IsEqual(T value, const std::vector<T>& allowed)
    {
        for (const auto& val : allowed)
        {
            if (value == val)
                return true;
        }

        return false;
    }

}
#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <initializer_list>

namespace Dynamite::Utils
{

    bool IsAlpha(char c, const std::initializer_list<char>& allowed = { });
    bool IsNumeric(char c, const std::initializer_list<char>& allowed = { });
    bool IsAlphaNumeric(char c, const std::initializer_list<char>& allowed = { });
    bool IsSpace(char c, const std::initializer_list<char>& allowed = { });
    
}
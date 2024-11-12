
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Definitions.hpp"

#include "Dynamite/Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // All
    /////////////////////////////////////////////////////////////////
    struct Program
    {
    public:
        std::vector<Definition> Definitions = { };
    };

}

#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"
#include "Dynamite/Parser/Nodes/Functions.hpp"
#include "Dynamite/Parser/Nodes/Statements.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // All
    /////////////////////////////////////////////////////////////////
    struct Program
    {
    public:
        using VariantType = Variant<Reference<VariableStatement>, Reference<Function>>;
    public:
        std::vector<VariantType> Definitions = { };
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string DefineToString(const Program::VariantType& obj, size_t indentLevel = 0);

}
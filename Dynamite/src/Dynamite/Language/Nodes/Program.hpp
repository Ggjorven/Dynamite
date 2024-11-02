
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"
#include "Dynamite/Language/Nodes/Functions.hpp"
#include "Dynamite/Language/Nodes/Statements.hpp"

#include "Dynamite/Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // All
    /////////////////////////////////////////////////////////////////
    struct Program
    {
    public:
        using VariantType = Variant<Ref<VariableStatement>, Ref<FunctionDeclaration>, Ref<FunctionDefinition>>;
    public:
        std::vector<VariantType> Definitions = { };
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string DefineToString(const Program::VariantType& obj, size_t indentLevel = 0);

}
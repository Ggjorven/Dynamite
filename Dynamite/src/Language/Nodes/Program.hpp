
#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Core.hpp"
#include "Language/Nodes/Functions.hpp"
#include "Language/Nodes/Statements.hpp"

#include "Language/Types/Type.hpp"

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
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <string>

namespace Dynamite::Language::Node
{

    // Note: VariableStatement, FunctionDefinition & FunctionDeclaration are defined elsewhere.
    /////////////////////////////////////////////////////////////////
    // Definitions 
    /////////////////////////////////////////////////////////////////
    
    //Variant<Ref<VariableStatement>, Ref<FunctionDeclaration>, Ref<FunctionDefinition>, Ref<NamespaceDefinition>, Ref<ClassDeclaration>, Ref<ClassDefinition>, Ref<StructDeclaration>, Ref<StructDefinition>, Ref<EnumDefinition>>;

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string DefineToString(const Definition& obj, size_t indentLevel = 0);

}
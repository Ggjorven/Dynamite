
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"
#include "Dynamite/Parser/Nodes/Statements.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // All
    /////////////////////////////////////////////////////////////////
    struct Function
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        Function(const Type& returnType = {}, const Token& name = {}, const std::vector<Reference<VariableStatement>>& parameters = { }, Reference<ScopeStatement> body = (Reference<ScopeStatement>)NullRef);

    public:
        Type ReturnType;
        Token Name;

        // Note: The expression can be a NullRef, since not every
        // arguments has a default value.
        std::vector<Reference<VariableStatement>> Parameters;

        Reference<ScopeStatement> Body;

        Type& GetType();
    };

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
    std::string FunctionToString(const Reference<Function> obj, size_t indentLevel = 0);
    std::string DefineToString(const Program::VariantType& obj, size_t indentLevel = 0);

}
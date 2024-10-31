#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // Functions
    /////////////////////////////////////////////////////////////////
    struct FunctionDeclaration
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        FunctionDeclaration(const Type& returnType = {}, const Token& name = {}, const std::vector<Reference<VariableStatement>>& parameters = { });

    public:
        Type ReturnType;
        Token Name;

        // Note: The expression can be a NullRef, since not every
        // arguments has a default value.
        std::vector<Reference<VariableStatement>> Parameters;

        Type GetType() const;
    };

    struct FunctionDefinition
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        FunctionDefinition(const Type& returnType = {}, const Token& name = {}, const std::vector<Reference<VariableStatement>>& parameters = { }, Reference<ScopeStatement> body = (Reference<ScopeStatement>)NullRef);

    public:
        Type ReturnType;
        Token Name;

        // Note: The expression can be a NullRef, since not every
        // arguments has a default value.
        std::vector<Reference<VariableStatement>> Parameters;

        Reference<ScopeStatement> Body;

        Type GetType() const;
    };

    struct Function // Note: Used to simplify function definitions
    {
    private:
        using VariantType = Variant<Reference<FunctionDeclaration>, Reference<FunctionDefinition>>;
        friend class Pulse::Memory::Control;
    private:
        Function(VariantType function = {});

    public:
        VariantType Func;

        Type GetType() const;
    };

    struct FunctionCall
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        FunctionCall(const Type& returnType = {}, const Token& function = {}, const std::vector<Reference<Expression>>& arguments = { }, size_t overloadIndex = 0);

    public:
        Type ReturnType;
        Token Function;

        std::vector<Reference<Expression>> Arguments;

        size_t OverloadIndex = 0;

        Type GetType() const;
        void SetType(const Type& type);
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string FunctionDeclarationToString(const Reference<FunctionDeclaration> obj, size_t indentLevel = 0);
    std::string FunctionDefinitionToString(const Reference<FunctionDefinition> obj, size_t indentLevel = 0);
    std::string FunctionCallToString(const Reference<FunctionCall> obj, size_t indentLevel = 0);

}
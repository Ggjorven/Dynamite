#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"
#include "Dynamite/Language/Utils/Namespace.hpp"

#include <string>

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Functions
    /////////////////////////////////////////////////////////////////
    struct FunctionDeclaration : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        FunctionDeclaration(const Type& returnType = {}, const std::string& name = {}, const std::vector<Ref<VariableStatement>>& parameters = { }, bool hasCStyleVardiadicArguments = false);

    public:
        Type ReturnType;
        std::string Name;

        // Note: The expression can be a NullRef, since not every
        // arguments has a default value.
        std::vector<Ref<VariableStatement>> Parameters;

        // Note: Still exists for LibC function support.
        bool CStyleVardiadicArguments = false;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::FunctionDeclaration; }
    };

    struct FunctionDefinition : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        FunctionDefinition(const Type& returnType = {}, const std::string& name = {}, const std::vector<Ref<VariableStatement>>& parameters = { }, Ref<ScopeStatement> body = (Ref<ScopeStatement>)NullRef);

    public:
        Type ReturnType;
        std::string Name;

        // Note: The expression can be a NullRef, since not every
        // arguments has a default value.
        std::vector<Ref<VariableStatement>> Parameters;

        Ref<ScopeStatement> Body;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::FunctionDefinition; }
    };

    struct Function : public Base // Note: Used to simplify function definitions
    {
    private:
        using VariantType = Variant<Ref<FunctionDeclaration>, Ref<FunctionDefinition>>;
        friend class Pulse::Memory::Control;
    private:
        Function(VariantType function = {});

    public:
        VariantType Func;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::Function; }
    };

    struct FunctionCall : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        FunctionCall(const Language::Namespace& nameSpace = {}, const Type& returnType = {}, const std::string& function = {}, const std::vector<Ref<Expression>>& arguments = {}, size_t overloadIndex = 0);

    public:
        Language::Namespace NamespaceLevel;

        Type ReturnType;
        std::string Function;

        std::vector<Ref<Expression>> Arguments;

        size_t OverloadIndex = 0;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::FunctionCall; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string FunctionDeclarationToString(const Ref<FunctionDeclaration> obj, size_t indentLevel = 0);
    std::string FunctionDefinitionToString(const Ref<FunctionDefinition> obj, size_t indentLevel = 0);
    std::string FunctionCallToString(const Ref<FunctionCall> obj, size_t indentLevel = 0);

}
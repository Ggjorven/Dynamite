#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/AccessSpecifier.hpp"
#include "Dynamite/Language/Types/Type.hpp"

#include <string>
#include <utility>

namespace Dynamite::Language::Node
{

    namespace Helper
    {
        template<typename T>
        using Member = std::pair<AccessSpecifier, Ref<T>>;
    }

    // Note: VariableStatement, FunctionDefinition & FunctionDeclaration are defined elsewhere.
    /////////////////////////////////////////////////////////////////
    // Definitions 
    /////////////////////////////////////////////////////////////////
    struct NamespaceDefinition : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        NamespaceDefinition(const std::string& identifier = {}, const std::vector<Definition>&definitions = {});

    public:
        std::string Identifier;
        std::vector<Definition> Definitions = { };

    public:
        inline static NodeType GetStaticType() { return NodeType::NamespaceDefinition; }
    };

    struct ClassDeclaration
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ClassDeclaration(const std::string& identifier = {});

    public:
        std::string Identifier;

    public:
        inline static NodeType GetStaticType() { return NodeType::ClassDeclaration; }
    };

    struct ClassDefinition
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ClassDefinition(const std::string& identifier = {}, const Map<std::string, Helper::Member<VariableStatement>>& variables = { }, const Map<std::string, Helper::Member<FunctionDefinition>>& functions = { });

    public:
        std::string Identifier;

        Map<std::string, Helper::Member<VariableStatement>> Variables;
        Map<std::string, Helper::Member<FunctionDefinition>> Functions;

    public:
        inline static NodeType GetStaticType() { return NodeType::ClassDefinition; }
    };

    struct StructDeclaration
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        StructDeclaration(const std::string& identifier = {});

    public:
        std::string Identifier;

    public:
        inline static NodeType GetStaticType() { return NodeType::StructDeclaration; }
    };

    struct StructDefinition
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        StructDefinition(const std::string& identifier = {}, const Map<std::string, Helper::Member<VariableStatement>>& variables = { }, const Map<std::string, Helper::Member<FunctionDefinition>>& functions = { });

    public:
        std::string Identifier;

        Map<std::string, Helper::Member<VariableStatement>> Variables;
        Map<std::string, Helper::Member<FunctionDefinition>> Functions;

    public:
        inline static NodeType GetStaticType() { return NodeType::StructDefinition; }
    };

    struct EnumDefinition
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        EnumDefinition(const std::string& identifier = {}, const Type& integerType = {}, const Map<std::string, std::string>& names = { });

    public:
        std::string Identifier;
        Type IntegerType;

        // Name & Integer value
        Map<std::string, std::string> Names;

    public:
        inline static NodeType GetStaticType() { return NodeType::EnumDefinition; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string NamespaceDefinitionToString(const Ref<NamespaceDefinition> obj, size_t indentLevel = 0);
    std::string ClassDeclarationToString(const Ref<ClassDeclaration> obj, size_t indentLevel = 0);
    std::string ClassDefinitionToString(const Ref<ClassDefinition> obj, size_t indentLevel = 0);
    std::string StructDeclarationToString(const Ref<StructDeclaration> obj, size_t indentLevel = 0);
    std::string StructDefinitionToString(const Ref<StructDefinition> obj, size_t indentLevel = 0);
    std::string EnumDefinitionToString(const Ref<EnumDefinition> obj, size_t indentLevel = 0);
    std::string DefinitionToString(const Definition& obj, size_t indentLevel = 0);

}
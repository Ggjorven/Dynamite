#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Core.hpp"

#include "Language/Nodes/Conditions.hpp"
#include "Language/Nodes/Expressions.hpp"

#include "Language/Types/Type.hpp"

#include <string>

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Statements
    /////////////////////////////////////////////////////////////////
    struct IfStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        IfStatement(Ref<Expression> expr = (Ref<Expression>)NullRef, Ref<ScopeStatement> scope = (Ref<ScopeStatement>)NullRef, Optional<Ref<ConditionBranch>> next = {});

    public:
        Ref<Expression> Expr;
        Ref<ScopeStatement> Scope;

        // Note: Optional next branch, can be else if or else
        // and that branch can contains additional branches.
        Optional<Ref<ConditionBranch>> Next;
    };

    struct VariableStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        VariableStatement(const Type& type = {}, const std::string& variable = {}, Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Type VariableType;
        std::string Variable;

        // What's the variable set to? Can be a NullRef.
        Ref<Expression> Expr;

        Type GetType() const;
    };

    struct ScopeStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ScopeStatement(const std::vector<Ref<Statement>>& statements = { });

    public:
        std::vector<Ref<Statement>> Statements;
    };

    struct AssignmentStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AssignmentStatement(const Type& variableType = {}, const std::string& variable = {}, Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Type VariableType;
        std::string Variable;

        Ref<Expression> Expr;

        Type GetType() const;
    };

    struct ReturnStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ReturnStatement(Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;

        Type GetType() const;
    };

    struct Statement
    {
    private:
        using VariantType = Variant<Ref<VariableStatement>, Ref<ScopeStatement>, Ref<IfStatement>, Ref<AssignmentStatement>, Ref<ReturnStatement>, Ref<FunctionCall>>;
        friend class Pulse::Memory::Control;
    private:
        Statement(VariantType statement = {});

    public:
        VariantType StatementObj;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string IfStatementToString(const Ref<IfStatement> obj, size_t indentLevel = 0);
    std::string VariableStatementToString(const Ref<VariableStatement> obj, size_t indentLevel = 0);
    std::string ScopeStatementToString(const Ref<ScopeStatement> obj, size_t indentLevel = 0);
    std::string AssignmentStatementToString(const Ref<AssignmentStatement> obj, size_t indentLevel = 0);
    std::string ReturnStatementToString(const Ref<ReturnStatement> obj, size_t indentLevel = 0);
    std::string StatementToString(const Ref<Statement> obj, size_t indentLevel = 0);

}
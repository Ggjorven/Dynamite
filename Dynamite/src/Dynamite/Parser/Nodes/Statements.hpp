
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"

#include "Dynamite/Parser/Nodes/Conditions.hpp"
#include "Dynamite/Parser/Nodes/Expressions.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // Statements
    /////////////////////////////////////////////////////////////////
    struct IfStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        IfStatement(Reference<Expression> expr = (Reference<Expression>)NullRef, Reference<ScopeStatement> scope = (Reference<ScopeStatement>)NullRef, Optional<Reference<ConditionBranch>> next = {});

    public:
        Reference<Expression> Expr;
        Reference<ScopeStatement> Scope;

        // Note: Optional next branch, can be else if or else
        // and that branch can contains additional branches.
        Optional<Reference<ConditionBranch>> Next;
    };

    struct VariableStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        VariableStatement(const Type& type = {}, const Token& variable = {}, Reference<Expression> expr = (Reference<Expression>)NullRef);

    public:
        Type VariableType;
        Token Variable;

        // What's the variable set to? Can be a NullRef.
        Reference<Expression> Expr;

        Type& GetType();
    };

    struct ScopeStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ScopeStatement(const std::vector<Reference<Statement>>& statements = { });

    public:
        std::vector<Reference<Statement>> Statements;
    };

    struct AssignmentStatement
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AssignmentStatement(const Type& variableType = {}, const Token& variable = {}, Reference<Expression> expr = (Reference<Expression>)NullRef);

    public:
        Type VariableType;
        Token Variable;

        Reference<Expression> Expr;

        Type& GetType();
    };

    struct Statement
    {
    private:
        using VariantType = Variant<Reference<VariableStatement>, Reference<ScopeStatement>, Reference<IfStatement>, Reference<AssignmentStatement>>;
        friend class Pulse::Memory::Control;
    private:
        Statement(VariantType statement = {});

    public:
        VariantType StatementObj;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string IfStatementToString(const Reference<IfStatement> obj, size_t indentLevel = 0);
    std::string VariableStatementToString(const Reference<VariableStatement> obj, size_t indentLevel = 0);
    std::string ScopeStatementToString(const Reference<ScopeStatement> obj, size_t indentLevel = 0);
    std::string AssignmentStatementToString(const Reference<AssignmentStatement> obj, size_t indentLevel = 0);
    std::string StatementToString(const Reference<Statement> obj, size_t indentLevel = 0);

}
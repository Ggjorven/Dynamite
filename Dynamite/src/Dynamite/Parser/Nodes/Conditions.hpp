#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // Conditions
    /////////////////////////////////////////////////////////////////
    struct ElseIfBranch
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ElseIfBranch(Reference<Expression> expr = (Reference<Expression>)NullRef, Reference<ScopeStatement> scope = (Reference<ScopeStatement>)NullRef, Optional<Reference<ConditionBranch>> next = {});

    public:
        Reference<Expression> Expr;
        Reference<ScopeStatement> Scope;

        // Note: Optional next branch, can be else if or else
        // Next can also contain more Next's.
        Optional<Reference<ConditionBranch>> Next;
    };

    struct ElseBranch
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ElseBranch(Reference<ScopeStatement> scope = (Reference<ScopeStatement>)NullRef);

    public:
        Reference<ScopeStatement> Scope;
    };

    struct ConditionBranch
    {
    private:
        using VariantType = Variant<Reference<ElseIfBranch>, Reference<ElseBranch>>;
        friend class Pulse::Memory::Control;
    private:
        ConditionBranch(VariantType branch = {});

    public:
        VariantType Branch;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string ElseIfBranchToString(const Reference<ElseIfBranch> obj, size_t indentLevel = 0);
    std::string ElseBranchToString(const Reference<ElseBranch> obj, size_t indentLevel = 0);
    std::string ConditionBranchToString(const Reference<ConditionBranch> obj, size_t indentLevel = 0);

}
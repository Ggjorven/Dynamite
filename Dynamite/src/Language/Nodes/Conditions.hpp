#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Core.hpp"

#include "Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Conditions
    /////////////////////////////////////////////////////////////////
    struct ElseIfBranch
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ElseIfBranch(Ref<Expression> expr = (Ref<Expression>)NullRef, Ref<ScopeStatement> scope = (Ref<ScopeStatement>)NullRef, Optional<Ref<ConditionBranch>> next = {});

    public:
        Ref<Expression> Expr;
        Ref<ScopeStatement> Scope;

        // Note: Optional next branch, can be else if or else
        // Next can also contain more Next's.
        Optional<Ref<ConditionBranch>> Next;
    };

    struct ElseBranch
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ElseBranch(Ref<ScopeStatement> scope = (Ref<ScopeStatement>)NullRef);

    public:
        Ref<ScopeStatement> Scope;
    };

    struct ConditionBranch
    {
    private:
        using VariantType = Variant<Ref<ElseIfBranch>, Ref<ElseBranch>>;
        friend class Pulse::Memory::Control;
    private:
        ConditionBranch(VariantType branch = {});

    public:
        VariantType Branch;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string ElseIfBranchToString(const Ref<ElseIfBranch> obj, size_t indentLevel = 0);
    std::string ElseBranchToString(const Ref<ElseBranch> obj, size_t indentLevel = 0);
    std::string ConditionBranchToString(const Ref<ConditionBranch> obj, size_t indentLevel = 0);

}
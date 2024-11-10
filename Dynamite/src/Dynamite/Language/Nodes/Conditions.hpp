#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Conditions
    /////////////////////////////////////////////////////////////////
    struct ElseIfBranch : public Base
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

    public:
        inline static NodeType GetStaticType() { return NodeType::ElseIfBranch; }
    };

    struct ElseBranch : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ElseBranch(Ref<ScopeStatement> scope = (Ref<ScopeStatement>)NullRef);

    public:
        Ref<ScopeStatement> Scope;

    public:
        inline static NodeType GetStaticType() { return NodeType::ElseBranch; }
    };

    struct ConditionBranch : public Base
    {
    private:
        using VariantType = Variant<Ref<ElseIfBranch>, Ref<ElseBranch>>;
        friend class Pulse::Memory::Control;
    private:
        ConditionBranch(VariantType branch = {});

    public:
        VariantType Branch;

    public:
        inline static NodeType GetStaticType() { return NodeType::ConditionBranch; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string ElseIfBranchToString(const Ref<ElseIfBranch> obj, size_t indentLevel = 0);
    std::string ElseBranchToString(const Ref<ElseBranch> obj, size_t indentLevel = 0);
    std::string ConditionBranchToString(const Ref<ConditionBranch> obj, size_t indentLevel = 0);

}
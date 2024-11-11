#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"
#include "Dynamite/Language/LiteralType.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <string>

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Booleans
    /////////////////////////////////////////////////////////////////
    struct AndAndBoolean : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AndAndBoolean(Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::AndAndBoolean; }
    };

    struct OrOrBoolean : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        OrOrBoolean(Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::OrOrBoolean; }
    };

    struct IsEqualBoolean : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        IsEqualBoolean(Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::IsEqualBoolean; }
    };

    struct MoreThanBoolean : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        MoreThanBoolean(Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::MoreThanBoolean; }
    };

    struct LessThanBoolean : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        LessThanBoolean(Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::LessThanBoolean; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string AndAndBooleanToString(const Ref<AndAndBoolean> obj, size_t indentLevel = 0);
    std::string OrOrBooleanToString(const Ref<OrOrBoolean> obj, size_t indentLevel = 0);
    std::string IsEqualBooleanToString(const Ref<IsEqualBoolean> obj, size_t indentLevel = 0);
    std::string MoreThanBooleanToString(const Ref<MoreThanBoolean> obj, size_t indentLevel = 0);
    std::string LessThanBooleanToString(const Ref<LessThanBoolean> obj, size_t indentLevel = 0);

}
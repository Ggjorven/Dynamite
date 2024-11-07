#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Operations
    /////////////////////////////////////////////////////////////////
    struct BinaryAddition : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryAddition(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryAddition; }
    };

    struct BinarySubtraction : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinarySubtraction(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinarySubtraction; }
    };

    struct BinaryMultiplication : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryMultiplication(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryMultiplication; }
    };

    struct BinaryDivision : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryDivision(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryDivision; }
    };

    struct BinaryOR : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryOR(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryOR; }
    };

    struct BinaryAND : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryAND(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryAND; }
    };

    struct BinaryXOR : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryXOR(const Type& type = {}, Ref<Expression> lhs = (Ref<Expression>)NullRef, Ref<Expression> rhs = (Ref<Expression>)NullRef);

    public:
        Type ResultType;

        Ref<Expression> LHS;
        Ref<Expression> RHS;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryXOR; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string BinaryAdditionToString(const Ref<BinaryAddition> obj, size_t indentLevel = 0);
    std::string BinarySubtractionToString(const Ref<BinarySubtraction> obj, size_t indentLevel = 0);
    std::string BinaryMultiplicationToString(const Ref<BinaryMultiplication> obj, size_t indentLevel = 0);
    std::string BinaryDivisionToString(const Ref<BinaryDivision> obj, size_t indentLevel = 0);
    std::string BinaryORToString(const Ref<BinaryOR> obj, size_t indentLevel = 0);
    std::string BinaryANDToString(const Ref<BinaryAND> obj, size_t indentLevel = 0);
    std::string BinaryXORToString(const Ref<BinaryXOR> obj, size_t indentLevel = 0);

}
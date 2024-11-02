#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Operations
    /////////////////////////////////////////////////////////////////
    struct BinaryAddition 
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
    };

    struct BinarySubtraction
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
    };

    struct BinaryMultiplication
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
    };

    struct BinaryDivision
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
    };

    struct BinaryOR
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
    };

    struct BinaryAND
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
    };

    struct BinaryXOR
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
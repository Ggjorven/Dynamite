#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // Operations
    /////////////////////////////////////////////////////////////////
    struct BinaryAddition 
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryAddition(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    struct BinarySubtraction
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinarySubtraction(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    struct BinaryMultiplication
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryMultiplication(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    struct BinaryDivision
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryDivision(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    struct BinaryOR
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryOR(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    struct BinaryAND
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryAND(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    struct BinaryXOR
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        BinaryXOR(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type GetType() const;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string BinaryAdditionToString(const Reference<BinaryAddition> obj, size_t indentLevel = 0);
    std::string BinarySubtractionToString(const Reference<BinarySubtraction> obj, size_t indentLevel = 0);
    std::string BinaryMultiplicationToString(const Reference<BinaryMultiplication> obj, size_t indentLevel = 0);
    std::string BinaryDivisionToString(const Reference<BinaryDivision> obj, size_t indentLevel = 0);
    std::string BinaryORToString(const Reference<BinaryOR> obj, size_t indentLevel = 0);
    std::string BinaryANDToString(const Reference<BinaryAND> obj, size_t indentLevel = 0);
    std::string BinaryXORToString(const Reference<BinaryXOR> obj, size_t indentLevel = 0);

    Optional<size_t> GetBinaryPrecendence(TokenType operation);
    size_t GetMaxBinaryPrecendence();

}
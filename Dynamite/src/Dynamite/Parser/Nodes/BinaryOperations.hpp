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
        friend class Control;
    private:
        BinaryAddition(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type& GetType();
    };

    struct BinarySubtraction
    {
    private:
        friend class Control;
    private:
        BinarySubtraction(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type& GetType();
    };

    struct BinaryMultiplication
    {
    private:
        friend class Control;
    private:
        BinaryMultiplication(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type& GetType();
    };

    struct BinaryDivision
    {
    private:
        friend class Control;
    private:
        BinaryDivision(const Type& type = {}, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);

    public:
        Type ResultType;

        Reference<Expression> LHS;
        Reference<Expression> RHS;

        Type& GetType();
    };

    // TODO: More binary operations

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string BinaryAdditionToString(const Reference<BinaryAddition> obj, size_t indentLevel = 0);
    std::string BinarySubtractionToString(const Reference<BinarySubtraction> obj, size_t indentLevel = 0);
    std::string BinaryMultiplicationToString(const Reference<BinaryMultiplication> obj, size_t indentLevel = 0);
    std::string BinaryDivisionToString(const Reference<BinaryDivision> obj, size_t indentLevel = 0);

}
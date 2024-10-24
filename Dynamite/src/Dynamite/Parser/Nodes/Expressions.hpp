
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"

#include "Dynamite/Parser/Nodes/Terms.hpp"
#include "Dynamite/Parser/Nodes/BinaryOperations.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

    /////////////////////////////////////////////////////////////////
    // Expressions
    /////////////////////////////////////////////////////////////////
    struct TermExpr
    {
    private:
        using VariantType = Variant<Reference<LiteralTerm>, Reference<IdentifierTerm>, Reference<ParenthesisTerm>>;
        friend class Pulse::Memory::Control;
    private:
        TermExpr(VariantType term = {});

    public:
        VariantType Term;

        Type& GetType();
    };
    
    struct BinaryExpr
    {
    private:
        using VariantType = Variant<Reference<BinaryAddition>, Reference<BinarySubtraction>, Reference<BinaryMultiplication>, Reference<BinaryDivision>>;
        friend class Pulse::Memory::Control;
    private:
        BinaryExpr(VariantType operation = {});
        BinaryExpr(TokenType operation, const Type& resultType, Reference<Expression> lhs, Reference<Expression> rhs);

    public:
        VariantType Operation;
        
        Type& GetType();
    };
    
    struct Expression
    {
    private:
        using VariantType = Variant<Reference<TermExpr>, Reference<BinaryExpr>>;
        friend class Pulse::Memory::Control;
    private:
        Expression(VariantType expr = {});

    public:
        VariantType Expr;

        Type& GetType();
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string TermExprToString(const Reference<TermExpr> obj, size_t indentLevel = 0);
    std::string BinaryExprToString(const Reference<BinaryExpr> obj, size_t indentLevel = 0);
    std::string ExpressionToString(const Reference<Expression> obj, size_t indentLevel = 0);

}
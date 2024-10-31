
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

        Type GetType() const;
        void SetType(const Type& type);

        bool IsLValue() const;
    };
    
    struct BinaryExpr
    {
    private:
        using VariantType = Variant<Reference<BinaryAddition>, Reference<BinarySubtraction>, Reference<BinaryMultiplication>, Reference<BinaryDivision>, Reference<BinaryOR>, Reference<BinaryAND>, Reference<BinaryXOR>>;
        friend class Pulse::Memory::Control;
    private:
        BinaryExpr(VariantType operation = {});
        BinaryExpr(TokenType operation, const Type& resultType, Reference<Expression> lhs, Reference<Expression> rhs);

    public:
        VariantType Operation;
        
        Type GetType() const;
        void SetType(const Type& type);
    };

    struct AddressExpr
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AddressExpr(Reference<Expression> expression = (Reference<Expression>)NullRef);

    public:
        Reference<Expression> Expr;

        Type GetType() const;
        void SetType(const Type& type); // Note: Just pass in the pointer type, the function will remove it.
    };

    struct DereferenceExpr
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        DereferenceExpr(Reference<Expression> expression = (Reference<Expression>)NullRef);

    public:
        Reference<Expression> Expr;

        Type GetType() const;
        void SetType(const Type& type); // Note: Just pass in the value type, the function will add a pointer to it.
    };
    
    struct Expression
    {
    private:
        using VariantType = Variant<Reference<TermExpr>, Reference<BinaryExpr>, Reference<FunctionCall>, Reference<AddressExpr>, Reference<DereferenceExpr>>;
        friend class Pulse::Memory::Control;
    private:
        Expression(VariantType expr = {});

    public:
        VariantType Expr;

        Type GetType() const;
        void SetType(const Type& type);

        bool IsLValue() const;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string TermExprToString(const Reference<TermExpr> obj, size_t indentLevel = 0);
    std::string BinaryExprToString(const Reference<BinaryExpr> obj, size_t indentLevel = 0);
    std::string AddressExprToString(const Reference<AddressExpr> obj, size_t indentLevel = 0);
    std::string DereferenceExprToString(const Reference<DereferenceExpr> obj, size_t indentLevel = 0);
    std::string ExpressionToString(const Reference<Expression> obj, size_t indentLevel = 0);

}
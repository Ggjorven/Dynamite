
#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Core.hpp"

#include "Language/Nodes/Terms.hpp"
#include "Language/Nodes/BinaryOperations.hpp"

#include "Language/OperationType.hpp"

#include "Language/Types/Type.hpp"

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Expressions
    /////////////////////////////////////////////////////////////////
    struct TermExpr
    {
    private:
        using VariantType = Variant<Ref<LiteralTerm>, Ref<IdentifierTerm>, Ref<ParenthesisTerm>>;
        friend class Pulse::Memory::Control;
    private:
        TermExpr(VariantType term = {});

    public:
        VariantType Term;

        Type GetType() const;

        bool IsLValue() const;
    };
    
    struct BinaryExpr
    {
    private:
        using VariantType = Variant<Ref<BinaryAddition>, Ref<BinarySubtraction>, Ref<BinaryMultiplication>, Ref<BinaryDivision>, Ref<BinaryOR>, Ref<BinaryAND>, Ref<BinaryXOR>>;
        friend class Pulse::Memory::Control;
    private:
        BinaryExpr(VariantType operation = {});
        BinaryExpr(OperationType operation, const Type& resultType, Ref<Expression> lhs, Ref<Expression> rhs);

    public:
        VariantType Operation;
        
        Type GetType() const;
    };

    struct AddressExpr
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AddressExpr(Ref<Expression> expression = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;

        Type GetType() const;
    };

    struct DereferenceExpr
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        DereferenceExpr(Ref<Expression> expression = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;

        Type GetType() const;
    };
    
    struct Expression
    {
    private:
        using VariantType = Variant<Ref<TermExpr>, Ref<BinaryExpr>, Ref<FunctionCall>, Ref<AddressExpr>, Ref<DereferenceExpr>>;
        friend class Pulse::Memory::Control;
    private:
        Expression(VariantType expr = {});

    public:
        VariantType Expr;

        Type GetType() const;

        bool IsLValue() const;
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string TermExprToString(const Ref<TermExpr> obj, size_t indentLevel = 0);
    std::string BinaryExprToString(const Ref<BinaryExpr> obj, size_t indentLevel = 0);
    std::string AddressExprToString(const Ref<AddressExpr> obj, size_t indentLevel = 0);
    std::string DereferenceExprToString(const Ref<DereferenceExpr> obj, size_t indentLevel = 0);
    std::string ExpressionToString(const Ref<Expression> obj, size_t indentLevel = 0);

}
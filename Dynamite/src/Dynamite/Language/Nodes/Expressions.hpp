
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"

#include "Dynamite/Language/Nodes/Terms.hpp"
#include "Dynamite/Language/Nodes/BinaryOperations.hpp"

#include "Dynamite/Language/OperationType.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <string>

namespace Dynamite::Language::Node
{

    /////////////////////////////////////////////////////////////////
    // Helper
    /////////////////////////////////////////////////////////////////
    namespace Helper
    {

        // Note: Will always be an LValue
        struct ResolvableTarget 
        {
        public:
            // Note: We store an identifier seperate from an expression,
            // because an identifier is not an LValue in our implementation.
            using VariantType = Variant<Ref<IdentifierTerm>, Ref<Expression>>;
        public:
            ResolvableTarget(VariantType target = {});

        public:
            VariantType Target;

            Type GetType() const;
        };

        std::string ResolvableTargetToString(const ResolvableTarget& target, size_t indentLevel = 0);
    }

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

    struct ReferenceExpr
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ReferenceExpr(const Helper::ResolvableTarget& target = {});

    public:
        Helper::ResolvableTarget Target;

        Type GetType() const;
    };

    struct AddressExpr
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AddressExpr(const Helper::ResolvableTarget& target = {});

    public:
        Helper::ResolvableTarget Target;

        Type GetType() const;
    };

    struct DereferenceExpr // Note: Dereferences references & pointers.
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        DereferenceExpr(const Helper::ResolvableTarget& target = {});

    public:
        Helper::ResolvableTarget Target;

        Type GetType() const;
    };
    
    struct Expression
    {
    private:
        using VariantType = Variant<Ref<TermExpr>, Ref<BinaryExpr>, Ref<FunctionCall>, Ref<ReferenceExpr>, Ref<AddressExpr>, Ref<DereferenceExpr>>;
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
    std::string ReferenceExprToString(const Ref<ReferenceExpr> obj, size_t indentLevel = 0);
    std::string AddressExprToString(const Ref<AddressExpr> obj, size_t indentLevel = 0);
    std::string DereferenceExprToString(const Ref<DereferenceExpr> obj, size_t indentLevel = 0);
    std::string ExpressionToString(const Ref<Expression> obj, size_t indentLevel = 0);

}
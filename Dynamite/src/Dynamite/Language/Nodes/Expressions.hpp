
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
    // Expressions
    /////////////////////////////////////////////////////////////////
    struct TermExpr : public Base
    {
    private:
        using VariantType = Variant<Ref<LiteralTerm>, Ref<IdentifierTerm>, Ref<ParenthesisTerm>>;
        friend class Pulse::Memory::Control;
    private:
        TermExpr(VariantType term = {});

    public:
        VariantType Term;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        bool IsLValue() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::TermExpr; }
    };
    
    struct BinaryExpr : public Base
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
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::BinaryExpr; }
    };

    struct ReferenceExpr : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ReferenceExpr(Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::ReferenceExpr; }
    };

    struct AddressExpr : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        AddressExpr(Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::AddressExpr; }
    };

    struct DereferenceExpr : public Base // Note: Dereferences references & pointers.
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        DereferenceExpr(Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::DereferenceExpr; }
    };

    struct CastExpr : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        CastExpr(Ref<Expression> expr = (Ref<Expression>)NullRef, const Type& type = {});

    public:
        Type ToType;
        Ref<Expression> Expr;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::CastExpr; }
    };

    struct BooleanExpr : public Base
    {
    private:
        using VariantType = Variant<Ref<AndAndBoolean>, Ref<OrOrBoolean>, Ref<IsEqualBoolean>, Ref<MoreThanBoolean>, Ref<LessThanBoolean>>;
        friend class Pulse::Memory::Control;
    private:
        BooleanExpr(VariantType boolean = {});

    public:
        VariantType Boolean;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::BooleanExpr; }
    };

    struct ArrayAccessExpr : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ArrayAccessExpr(Ref<Expression> arrayExpr = (Ref<Expression>)NullRef, Ref<Expression> indexExpr = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Array;
        Ref<Expression> Index;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::ArrayAccessExpr; }
    };
    
    struct Expression : public Base
    {
    private:
        using VariantType = Variant<Ref<TermExpr>, Ref<BinaryExpr>, Ref<FunctionCall>, Ref<ReferenceExpr>, Ref<AddressExpr>, Ref<DereferenceExpr>, Ref<CastExpr>, Ref<BooleanExpr>, Ref<ArrayAccessExpr>>;
        friend class Pulse::Memory::Control;
    private:
        Expression(VariantType expr = {});

    public:
        VariantType Expr;

        Type GetType() const;
        NodeType GetUnderlyingType() const;

        bool IsLValue() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::Expression; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string TermExprToString(const Ref<TermExpr> obj, size_t indentLevel = 0);
    std::string BinaryExprToString(const Ref<BinaryExpr> obj, size_t indentLevel = 0);
    std::string ReferenceExprToString(const Ref<ReferenceExpr> obj, size_t indentLevel = 0);
    std::string AddressExprToString(const Ref<AddressExpr> obj, size_t indentLevel = 0);
    std::string DereferenceExprToString(const Ref<DereferenceExpr> obj, size_t indentLevel = 0);
    std::string CastExprToString(const Ref<CastExpr> obj, size_t indentLevel = 0);
    std::string BooleanExprToString(const Ref<BooleanExpr> obj, size_t indentLevel = 0);
    std::string ArrayAccessExprToString(const Ref<ArrayAccessExpr> obj, size_t indentLevel = 0);
    std::string ExpressionToString(const Ref<Expression> obj, size_t indentLevel = 0);

}
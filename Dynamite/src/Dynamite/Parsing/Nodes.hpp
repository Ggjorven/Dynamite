#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Variables.hpp"

#include <Pulse/Memory/ArenaAllocator.hpp>

#include <vector>
#include <variant>
#include <string>
#include <functional>

namespace Dynamite::Node
{

    template<typename ...Types>
    using Variant = std::variant<Types...>;

    // Internal reference type.
    template<typename T>
    using Reference = T*;

    constexpr const Reference<void> NullRef = nullptr;
	
	/////////////////////////////////////////////////////////////////
	struct Expression;

	// Expression types
	struct TermExpr;
	struct BinaryExpr;

    // Term variants
	struct IdentifierTerm;
	struct IntegerLiteralTerm;
	//struct FloatLiteralTerm;
	//struct CharLiteralTerm;
	//struct StringLiteralTerm;

    // Binary is not a variant, since we just store the
    // operator and the LHS & RHS, and can switch based on that.

	struct Statement;

	// Statement types
	struct VariableStatement;
	struct ExitStatement;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    struct IntegerLiteralTerm
    {
    private:
        IntegerLiteralTerm(const Token& token);
    
    public:
        Token TokenObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<IntegerLiteralTerm> New(const Token& token = {});
    };

    struct IdentifierTerm
    {
    private:
        IdentifierTerm(const Token& token);
    
    public:
        Token TokenObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<IdentifierTerm> New(const Token& token = {});
    };

    struct TermExpr
    {
    private:
        TermExpr(Reference<IntegerLiteralTerm> integerLiteral);
        TermExpr(Reference<IdentifierTerm> identifier);

    public:
        Variant<Reference<IntegerLiteralTerm>, Reference<IdentifierTerm>> TermObj;

        [[nodiscard]] Token GetToken();

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<TermExpr> New(Reference<IntegerLiteralTerm> integerLiteral = (Reference<IntegerLiteralTerm>)NullRef);
        [[nodiscard]] static Reference<TermExpr> New(Reference<IdentifierTerm> identifier = (Reference<IdentifierTerm>)NullRef);
    };
	/////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    struct BinaryExpr
    {
    public:
        // TODO: Binary operators |, &, ^
        enum class Type : uint8_t
        {
            None = 0,
            Addition = TokenType::Plus,
            Subtraction = TokenType::Minus,
            Multiplication = TokenType::Star,
            Division = TokenType::Divide,
        };
    private:
        BinaryExpr(Type binaryType, Reference<Expression> lhs, Reference<Expression> rhs);

    public:
        Type BinaryType;
        Reference<Expression> LHS;
        Reference<Expression> RHS;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<BinaryExpr> New(Type binaryType = Type::None, Reference<Expression> lhs = (Reference<Expression>)NullRef, Reference<Expression> rhs = (Reference<Expression>)NullRef);
    };
	/////////////////////////////////////////////////////////////////
    
    /////////////////////////////////////////////////////////////////
    struct Expression
    {
    private:
        Expression(Reference<TermExpr> term, ValueType type);
        Expression(Reference<BinaryExpr> binary, ValueType type);

    public:
        ValueType Type;
        Variant<Reference<TermExpr>, Reference<BinaryExpr>> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<Expression> New(Reference<TermExpr> term = (Reference<TermExpr>)NullRef, ValueType type = ValueType::None);
        [[nodiscard]] static Reference<Expression> New(Reference<BinaryExpr> binary = (Reference<BinaryExpr>)NullRef, ValueType type = ValueType::None);
    };
	/////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
    struct VariableStatement
    {
    private:
        VariableStatement(const Token& token, Reference<Expression> expr);

    public:
        Token TokenObj;
        Reference<Expression> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<VariableStatement> New(const Token& token = {}, Reference<Expression> expr = (Reference<Expression>)NullRef);
    };

    struct ExitStatement
    {
    private:
        ExitStatement(Reference<Expression> expr);

    public:
        Reference<Expression> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<ExitStatement> New(Reference<Expression> expr = (Reference<Expression>)NullRef);
    };

    struct Statement
    {
    private:
        Statement(Reference<VariableStatement> var);
        Statement(Reference<ExitStatement> exit);

    public:
        Variant<Reference<VariableStatement>, Reference<ExitStatement>> StatementObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<Statement> New(Reference<VariableStatement> var = (Reference<VariableStatement>)NullRef);
        [[nodiscard]] static Reference<Statement> New(Reference<ExitStatement> exit = (Reference<ExitStatement>)NullRef);
    };
	/////////////////////////////////////////////////////////////////
    
	/////////////////////////////////////////////////////////////////
    struct Program
    {
    public:
        std::vector<Reference<Statement>> Statements = { };
    };
	/////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string FormatExpressionData(const Reference<Expression> expr);
    std::string FormatStatementData(const Reference<Statement> statement);

}
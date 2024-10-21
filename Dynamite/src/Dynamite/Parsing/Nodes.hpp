#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Variables.hpp"

#include <Pulse/Memory/ArenaAllocator.hpp>

#include <vector>
#include <variant>
#include <string>
#include <optional>
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
	struct LiteralTerm;
    struct ParenthesisTerm;

    // Binary is not a variant, since we just store the
    // operator and the LHS & RHS, and can switch based on that.

	struct Statement;

	// Statement types
    struct ElseIfBranch;
    struct ElseBranch;
    struct ScopeStatement;
    struct ConditionBranch; // Note: This is just for wrapping else if and else statements
    struct IfStatement;

	struct VariableStatement;
	struct ExitStatement;
    struct AssignmentStatement;
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    struct LiteralTerm
    {
    public:
        enum class Type : uint8_t
        {
            None = 0,
            Bool = (uint8_t)TokenType::BoolLiteral,
            Integer = (uint8_t)TokenType::IntegerLiteral,
            Float = (uint8_t)TokenType::FloatLiteral,
            Char = (uint8_t)TokenType::CharLiteral,
            String = (uint8_t)TokenType::StringLiteral,
        };
    private:
        LiteralTerm(Type literalType, const Token& token);

    public:
        Type LiteralType;
        Token TokenObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<LiteralTerm> New(Type literalType = Type::None, const Token& token = {});
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

    struct ParenthesisTerm // Note: We have this as a seperate term, to support expressions within parenthesis.
    {
    private:
        ParenthesisTerm(Reference<Expression> expr);

    public:
        Reference<Expression> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<ParenthesisTerm> New(Reference<Expression> expr);
    };

    struct TermExpr
    {
    private:
        TermExpr(Variant<Reference<LiteralTerm>, Reference<IdentifierTerm>, Reference<ParenthesisTerm>> term);

    public:
        Variant<Reference<LiteralTerm>, Reference<IdentifierTerm>, Reference<ParenthesisTerm>> TermObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<TermExpr> New(Variant<Reference<LiteralTerm>, Reference<IdentifierTerm>, Reference<ParenthesisTerm>> = {});
    };
	/////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    struct BinaryExpr
    {
    public:
        enum class Type : uint8_t
        {
            None = 0,
            Addition = (uint8_t)TokenType::Plus,
            Subtraction = (uint8_t)TokenType::Minus,
            Multiplication = (uint8_t)TokenType::Multiply,
            Division = (uint8_t)TokenType::Divide,

            Or = (uint8_t)TokenType::Or,
            And = (uint8_t)TokenType::And,
            Xor = (uint8_t)TokenType::Xor,
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
        Expression(ValueType type, Variant<Reference<TermExpr>, Reference<BinaryExpr>> expr);

    public:
        ValueType Type;
        Variant<Reference<TermExpr>, Reference<BinaryExpr>> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<Expression> New(ValueType type = ValueType::None, Variant<Reference<TermExpr>, Reference<BinaryExpr>> expr = {});
    };
	/////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////
    struct ElseIfBranch
    {
    private:
        ElseIfBranch(Reference<Expression> expr, Reference<ScopeStatement> scope, std::optional<Reference<ConditionBranch>> next);

    public:
        Reference<Expression> ExprObj;
        Reference<ScopeStatement> Scope;

        // Note: Optional next branch, can be else if or else.
        std::optional<Reference<ConditionBranch>> Next;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<ElseIfBranch> New(Reference<Expression> expr = (Reference<Expression>)NullRef, Reference<ScopeStatement> scope = (Reference<ScopeStatement>)NullRef, std::optional<Reference<ConditionBranch>> next = {});
    };
    
    struct ElseBranch
    {
    private:
        ElseBranch(Reference<ScopeStatement> scope);

    public:
        Reference<ScopeStatement> Scope;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<ElseBranch> New(Reference<ScopeStatement> scope = (Reference<ScopeStatement>)NullRef);
    };

    struct ConditionBranch
    {
    private:
        ConditionBranch(Variant<Reference<ElseIfBranch>, Reference<ElseBranch>> branch);

    public:
        Variant<Reference<ElseIfBranch>, Reference<ElseBranch>> ConditionObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<ConditionBranch> New(Variant<Reference<ElseIfBranch>, Reference<ElseBranch>> branch = {});
    };

    struct IfStatement
    {
    private:
        IfStatement(Reference<Expression> expr, Reference<ScopeStatement> scope, std::optional<Reference<ConditionBranch>> next);

    public:
        Reference<Expression> ExprObj;
        Reference<ScopeStatement> Scope;

        // Note: Optional next branch, can be else if or else.
        std::optional<Reference<ConditionBranch>> Next;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<IfStatement> New(Reference<Expression> expr = (Reference<Expression>)NullRef, Reference<ScopeStatement> scope = (Reference<ScopeStatement>)NullRef, std::optional<Reference<ConditionBranch>> next = {});
    };
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    struct VariableStatement
    {
    private:
        VariableStatement(ValueType type, const Token& token, Reference<Expression> expr);

    public:
        ValueType Type;
        Token TokenObj;
        Reference<Expression> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<VariableStatement> New(ValueType type = ValueType::None, const Token& token = {}, Reference<Expression> expr = (Reference<Expression>)NullRef);
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

    struct ScopeStatement
    {
    private:
        ScopeStatement(const std::vector<Reference<Statement>>& statements);

    public:
        std::vector<Reference<Statement>> Statements;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<ScopeStatement> New(const std::vector<Reference<Statement>>& statements = {});
    };

    struct AssignmentStatement
    {
    private:
        AssignmentStatement(const Token& token, Reference<Expression> expr);

    public:
        Token TokenObj;
        Reference<Expression> ExprObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<AssignmentStatement> New(const Token& token = {}, Reference<Expression> expr = (Reference<Expression>)NullRef);
    };

    struct Statement
    {
    private:
        Statement(Variant<Reference<VariableStatement>, Reference<ExitStatement>, Reference<ScopeStatement>, Reference<IfStatement>, Reference<AssignmentStatement>> statement);

    public:
        Variant<Reference<VariableStatement>, Reference<ExitStatement>, Reference<ScopeStatement>, Reference<IfStatement>, Reference<AssignmentStatement>> StatementObj;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<Statement> New(Variant<Reference<VariableStatement>, Reference<ExitStatement>, Reference<ScopeStatement>, Reference<IfStatement>, Reference<AssignmentStatement>> statement = {});
    };
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
    struct Function
    {
    private:
        Function(ValueType returnType, const Token& name, const std::vector<Reference<VariableStatement>>& parameters, Reference<ScopeStatement> body);

    public:
        ValueType ReturnType;
        Token Name;
    
        // Note: The expression can be a NullRef, since not every
        // arguments has a default value.
        std::vector<Reference<VariableStatement>> Parameters;

        Reference<ScopeStatement> Body;

    public: // Custom allocator functions.
        template<typename T, typename ...TArgs>
        friend T* Pulse::Memory::DynamicArenaAllocator::Construct(TArgs&& ...args);

        [[nodiscard]] static Reference<Function> New(ValueType returnType = ValueType::None, const Token& name = {}, const std::vector<Reference<VariableStatement>>& parameters = { }, Reference<ScopeStatement> body = (Reference<ScopeStatement>)NullRef);
    };

    struct Program
    {
    public:
        using Define = Variant<Reference<VariableStatement>, Reference<Function>>;
        std::vector<Define> Defines = { };
    };
	/////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::optional<size_t> GetBinaryExprPrecendce(BinaryExpr::Type type);

    std::string FormatExpressionData(const Reference<Expression> expr);
    std::string FormatConditionBranch(const Reference<ConditionBranch> branch);
    std::string FormatStatementData(const Reference<Statement> statement);
    std::string FormatFunction(const Reference<Function> function);

    std::string FormatDefine(const Program::Define& define);

}

#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include <vector>
#include <variant>
#include <string>

namespace Dynamite::Nodes
{

	template<typename ...Types>
	using Variant = std::variant<Types...>;

    /////////////////////////////////////////////////////////////////
    // Expression nodes
    /////////////////////////////////////////////////////////////////
    struct Expression
    {
    public:
        enum class Type : uint8_t { None = 0, Int64Literal, Identifier, Binary };
    public:
        struct Int64Literal
        {
        public:
            Token TokenObj = {};

        public:
            Int64Literal() = default;
            Int64Literal(const Token& token);
            ~Int64Literal() = default;

            static Int64Literal* New();
            static Int64Literal* New(const Token& token);
        };

        struct Identifier
        {
        public:
            Token TokenObj = {};

        public:
            Identifier() = default;
            Identifier(const Token& token);
            ~Identifier() = default;

            static Identifier* New();
            static Identifier* New(const Token& token);
        };

        struct Binary
        {
        public:
            enum class Type : uint8_t { None = 0, Addition, Multiply };
        public:
            struct Addition
            {
            public:
                Expression* LHS = nullptr;
                Expression* RHS = nullptr;

            public:
                Addition() = default;
                Addition(Expression* lhs, Expression* rhs);
                ~Addition() = default;

                static Addition* New();
                static Addition* New(Expression* lhs, Expression* rhs);
            };

            struct Multiply
            {
            public:
                Expression* LHS = nullptr;
                Expression* RHS = nullptr;

            public:
                Multiply() = default;
                Multiply(Expression* lhs, Expression* rhs);
                ~Multiply() = default;

                static Multiply* New();
                static Multiply* New(Expression* lhs, Expression* rhs);
            };

        public:
            Binary() = default;
            Binary(Addition* addition);
            Binary(Multiply* multiply);
            ~Binary() = default;

            static Binary* New();
            static Binary* New(Addition* addition);
            static Binary* New(Multiply* multiply);

        public:
            Type BinaryType = Type::None;

            union
            {
                Addition* AdditionObj = nullptr;
                Multiply* MultiplyObj;
            };
        };

    public:
        Expression() = default;
        Expression(Int64Literal* int64literal);
        Expression(Identifier* identifier);
        Expression(Binary* binary);
        ~Expression() = default;

        static Expression* New();
        static Expression* New(Int64Literal* int64literal);
        static Expression* New(Identifier* identifier);
        static Expression* New(Binary* binary);

    public:
        Type ExpressionType = Type::None;

        union
        {
            Int64Literal* Int64LiteralObj = nullptr;
            Identifier* IdentifierObj;
            Binary* BinaryObj;
        };
    }; // End of Expression

    std::string FormatExpressionData(const Expression& expr);

    /////////////////////////////////////////////////////////////////
    // Statement nodes
    /////////////////////////////////////////////////////////////////
    struct Statement
    {
    public:
        enum class Type : uint8_t { None = 0, Let, Exit };
    public:
        struct Let
        {
        public:
            Token TokenObj = {};
            Expression* ExpressionObj = nullptr;

        public:
            Let() = default;
            Let(const Token& token, Expression* expression);
            ~Let() = default;

            static Let* New();
            static Let* New(const Token& token, Expression* expression = nullptr);
        };

        struct Exit
        {
        public:
            Expression* ExpressionObj = nullptr;

        public:
            Exit() = default;
            Exit(Expression* expression);
            ~Exit() = default;

            static Exit* New();
            static Exit* New(Expression* expression);
        };

    public:
        Statement() = default;
        Statement(Let* let);
        Statement(Exit* exit);
        ~Statement() = default;

        static Statement* New();
        static Statement* New(Let* let);
        static Statement* New(Exit* exit);

    public:
        Type StatementType = Type::None;

        union
        {
            Let* LetObj = nullptr;
            Exit* ExitObj;
        };
    }; // End of statement

    std::string FormatStatementData(const Statement& statement);

    /////////////////////////////////////////////////////////////////
    // Main program node
    /////////////////////////////////////////////////////////////////
    struct Program 
    {
    public:
        std::vector<Statement*> Statements = { };
    };


}
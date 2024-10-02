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
	struct Int64Literal
	{
	public:
        Token TokenObject = {};
	};

    struct Identifier
    {
    public:
        Token TokenObject = {};
    };

    using Expression = Variant<Identifier, Int64Literal>;
    std::string ExpressionToString(const Expression& expr);
    std::string FormatExpressionData(const Expression& expr);

    /////////////////////////////////////////////////////////////////
    // Statement nodes
    /////////////////////////////////////////////////////////////////
    struct LetStatement 
    {
    public:
        Token TokenObject = {};
        Expression ExpressionObject = {};
    };

    struct ExitStatement 
    {
    public:
        Expression ExpressionObject = {};
    };

    using Statement = Variant<LetStatement, ExitStatement>;
    std::string StatementToString(const Statement& statement);
    std::string FormatStatementData(const Statement& statement);

    /////////////////////////////////////////////////////////////////
    // Main program node
    /////////////////////////////////////////////////////////////////
    struct Program 
    {
    public:
        std::vector<Statement> Statements = { };
    };


}
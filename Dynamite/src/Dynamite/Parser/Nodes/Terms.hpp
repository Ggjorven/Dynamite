#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/Core.hpp"

#include "Dynamite/Types/Type.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Terms
	/////////////////////////////////////////////////////////////////
    struct LiteralTerm
    {
    private:
        friend class Control;
    private:
        LiteralTerm(const Type& type = {}, const Token& lit = {});

    public:
        Type LiteralType;
        Token Literal;

        Type& GetType();
    };

    struct IdentifierTerm
    {
    private:
        friend class Control;
    private:
        IdentifierTerm(const Type& type = {}, const Token& identifier = {});

    public:
        Type IdentifierType;
        Token Identifier;

        Type& GetType();
    };

    // Note: We have this as a seperate term, to support expressions within parenthesis.
    struct ParenthesisTerm 
    {
    private:
        friend class Control;
    private:
        ParenthesisTerm(Reference<Expression> expr = (Reference<Expression>)NullRef);

    public:
        Reference<Expression> Expr;
        
        Type& GetType();
    };

    // TODO: Function (call) term

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string LiteralTermToString(const Reference<LiteralTerm> obj, size_t indentLevel = 0);
    std::string IdentifierTermToString(const Reference<IdentifierTerm> obj, size_t indentLevel = 0);
    std::string ParenthesisTermToString(const Reference<ParenthesisTerm> obj, size_t indentLevel = 0);

}
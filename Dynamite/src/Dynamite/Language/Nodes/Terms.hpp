#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Core.hpp"
#include "Dynamite/Language/LiteralType.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <string>

namespace Dynamite::Language::Node
{

	/////////////////////////////////////////////////////////////////
	// Terms
	/////////////////////////////////////////////////////////////////
    struct LiteralTerm : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        LiteralTerm(const Type& type = {}, LiteralType litType = LiteralType::None, const std::string& lit = {});

    public:
        Type ValueType;

        LiteralType LitType;
        std::string Literal;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::LiteralTerm; }
    };

    struct IdentifierTerm : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        IdentifierTerm(const Type& type = {}, const std::string& identifier = {});

    public:
        Type IdentifierType;
        std::string Identifier;

        Type GetType() const;

    public:
        inline static NodeType GetStaticType() { return NodeType::IdentifierTerm; }
    };

    // Note: We have this as a seperate term, to support expressions within parenthesis.
    struct ParenthesisTerm : public Base
    {
    private:
        friend class Pulse::Memory::Control;
    private:
        ParenthesisTerm(Ref<Expression> expr = (Ref<Expression>)NullRef);

    public:
        Ref<Expression> Expr;
        
        Type GetType() const;
        NodeType GetUnderlyingType() const;

        bool IsLValue() const;

        Ref<Base> GetUnderlying();

    public:
        inline static NodeType GetStaticType() { return NodeType::ParenthesisTerm; }
    };

    /////////////////////////////////////////////////////////////////
    // Helper functions
    /////////////////////////////////////////////////////////////////
    std::string LiteralTermToString(const Ref<LiteralTerm> obj, size_t indentLevel = 0);
    std::string IdentifierTermToString(const Ref<IdentifierTerm> obj, size_t indentLevel = 0);
    std::string ParenthesisTermToString(const Ref<ParenthesisTerm> obj, size_t indentLevel = 0);

}
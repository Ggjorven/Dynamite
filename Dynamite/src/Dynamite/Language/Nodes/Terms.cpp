#include "dypch.h"
#include "Terms.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Language/Nodes/Expressions.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

namespace Dynamite::Language::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	LiteralTerm::LiteralTerm(const Type& type, LiteralType litType, const std::string& lit)
		: ValueType(type), LitType(litType), Literal(lit)
	{
	}

	IdentifierTerm::IdentifierTerm(const Type& type, const std::string& ident)
		: IdentifierType(type), Identifier(ident)
	{
	}

	ParenthesisTerm::ParenthesisTerm(Ref<Expression> expr) 
		: Expr(expr)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type LiteralTerm::GetType() const
	{
		return ValueType;
	}

	Type IdentifierTerm::GetType() const
	{
		return IdentifierType;
	}

	Type ParenthesisTerm::GetType() const
	{
		return Expr->GetType();
	}

	NodeType ParenthesisTerm::GetUnderlyingType() const
	{
		return Expr->GetUnderlyingType();
	}

	bool ParenthesisTerm::IsLValue() const
	{
		return Expr->IsLValue();
	}

	Ref<Base> ParenthesisTerm::GetUnderlying()
	{
		return Expr->GetUnderlying();
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string LiteralTermToString(const Ref<LiteralTerm> obj, size_t indentLevel)
	{
		if (obj == (Ref<LiteralTerm>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);
		
		std::string type = "<undefined literal name>";
		switch (obj->LitType)
		{
		case LiteralType::BoolLiteral:
			type = "Bool";
			break;
		case LiteralType::IntegerLiteral:
			type = "Integer";
			break;
		case LiteralType::FloatLiteral:
			type = "Float";
			break;
		case LiteralType::CharLiteral:
			type = "Char";
			break;
		case LiteralType::CharArrayLiteral:
			type = "CharArray";
			break;

		default:
			break;
		}

		str += Pulse::Text::Format("([LiteralTerm::{0}({1})] = '{2}')", type, TypeCollection::ToString(obj->GetType()), obj->Literal);

		return str;
	}

	std::string IdentifierTermToString(const Ref<IdentifierTerm> obj, size_t indentLevel)
	{
		if (obj == (Ref<IdentifierTerm>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([IdentifierTerm({0})] = '{1}')", TypeCollection::ToString(obj->GetType()), obj->Identifier);

		return str;
	}

	std::string ParenthesisTermToString(const Ref<ParenthesisTerm> obj, size_t indentLevel)
	{
		if (obj == (Ref<ParenthesisTerm>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([ParenthesisTerm({0})] = '\n{1}'\n{2})", TypeCollection::ToString(obj->GetType()), exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

}
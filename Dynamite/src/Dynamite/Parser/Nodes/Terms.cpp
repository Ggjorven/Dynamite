#include "dypch.h"
#include "Terms.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Parser/Nodes/Expressions.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	LiteralTerm::LiteralTerm(const Type& type, const Token& lit)
		: LiteralType(type), Literal(lit)
	{
	}

	IdentifierTerm::IdentifierTerm(const Type& type, const Token& ident)
		: IdentifierType(type), Identifier(ident)
	{
	}

	ParenthesisTerm::ParenthesisTerm(Reference<Expression> expr) 
		: Expr(expr)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type LiteralTerm::GetType() const
	{
		return LiteralType;
	}

	Type IdentifierTerm::GetType() const
	{
		return IdentifierType;
	}

	Type ParenthesisTerm::GetType() const
	{
		return Expr->GetType();
	}

	bool ParenthesisTerm::IsLValue() const
	{
		return Expr->IsLValue();
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string LiteralTermToString(const Reference<LiteralTerm> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);
		
		std::string type = "<undefined literal name>";
		switch (obj->Literal.Type)
		{
		case TokenType::BoolLiteral:
			type = "Bool";
			break;
		case TokenType::IntegerLiteral:
			type = "Integer";
			break;
		case TokenType::FloatLiteral:
			type = "Float";
			break;
		case TokenType::CharLiteral:
			type = "Char";
			break;
		case TokenType::CharArrayLiteral:
			type = "CharArray";
			break;

		default:
			break;
		}

		str += Pulse::Text::Format("([LiteralTerm::{0}({1})] = '{2}')", type, TypeSystem::ToString(obj->GetType()), obj->Literal.Value);

		return str;
	}

	std::string IdentifierTermToString(const Reference<IdentifierTerm> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([IdentifierTerm({0})] = '{1}')", TypeSystem::ToString(obj->GetType()), obj->Identifier.Value);

		return str;
	}

	std::string ParenthesisTermToString(const Reference<ParenthesisTerm> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([ParenthesisTerm({0})] = '\n{1}'\n{2})", TypeSystem::ToString(obj->GetType()), exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

}
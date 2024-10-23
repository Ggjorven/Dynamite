#include "dypch.h"
#include "Terms.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Parser/Nodes/Expressions.hpp"

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
	Type& LiteralTerm::GetType()
	{
		return LiteralType;
	}

	Type& IdentifierTerm::GetType()
	{
		return IdentifierType;
	}

	Type& ParenthesisTerm::GetType()
	{
		return Expr->GetType();
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string LiteralTermToString(const Reference<LiteralTerm> obj, size_t indentLevel)
	{
		std::string str(indentLevel, '\t');
		
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

		str += Pulse::Text::Format("([LiteralTerm::{0}({1})] = '{2}')", type, TypeToString(obj->GetType()), obj->Literal.Value);

		return str;
	}

	std::string IdentifierTermToString(const Reference<IdentifierTerm> obj, size_t indentLevel)
	{
		std::string str(indentLevel, '\t');

		str += Pulse::Text::Format("([IdentifierTerm({0})] = '{1}')", TypeToString(obj->GetType()), obj->Identifier.Value);

		return str;
	}

	std::string ParenthesisTermToString(const Reference<ParenthesisTerm> obj, size_t indentLevel)
	{
		std::string str(indentLevel, '\t');

		str += Pulse::Text::Format("([ParenthesisTerm({0})] = '(\n{1}\n{2})')", TypeToString(obj->GetType()), ExpressionToString(obj->Expr, indentLevel + 1), std::string(indentLevel, '\t'));

		return str;
	}

}
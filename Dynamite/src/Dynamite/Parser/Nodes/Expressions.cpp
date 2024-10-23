#include "dypch.h"
#include "Expressions.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	TermExpr::TermExpr(VariantType term)
		: Term(term)
	{
	}

	BinaryExpr::BinaryExpr(VariantType operation)
		: Operation(operation)
	{
	}

	Expression::Expression(VariantType expr)
		: Expr(expr)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type& TermExpr::GetType()
	{
		struct TermVisitor
		{
			Type& operator () (const Reference<LiteralTerm> obj) const
			{
				return obj->GetType();
			}
			Type& operator () (const Reference<IdentifierTerm> obj) const
			{
				return obj->GetType();
			}
			Type& operator () (const Reference<ParenthesisTerm> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	Type& BinaryExpr::GetType()
	{
		struct OperationVisitor
		{
			Type& operator () (const Reference<BinaryAddition> obj) const
			{
				return obj->GetType();
			}
			Type& operator () (const Reference<BinarySubtraction> obj) const
			{
				return obj->GetType();
			}
			Type& operator () (const Reference<BinaryMultiplication> obj) const
			{
				return obj->GetType();
			}
			Type& operator () (const Reference<BinaryDivision> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(OperationVisitor(), Operation);
	}

	Type& Expression::GetType()
	{
		struct ExpressionVisitor
		{
			Type& operator () (const Reference<TermExpr> obj) const
			{
				return obj->GetType();
			}
			Type& operator () (const Reference<BinaryExpr> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(ExpressionVisitor(), Expr);
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TermExprToString(const Reference<TermExpr> obj, size_t indentLevel)
	{
		struct TermExprVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<LiteralTerm> obj) const
			{
				return LiteralTermToString(obj, Indent);
			}
			std::string operator () (const Reference<IdentifierTerm> obj) const
			{
				return IdentifierTermToString(obj, Indent);
			}
			std::string operator () (const Reference<ParenthesisTerm> obj) const
			{
				return ParenthesisTermToString(obj, Indent);
			}
		};

		return std::visit(TermExprVisitor(indentLevel), obj->Term);
	}

	std::string BinaryExprToString(const Reference<BinaryExpr> obj, size_t indentLevel)
	{
		struct BinaryExprVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<BinaryAddition> obj) const
			{
				return BinaryAdditionToString(obj, Indent);
			}
			std::string operator () (const Reference<BinarySubtraction> obj) const
			{
				return BinarySubtractionToString(obj, Indent);
			}
			std::string operator () (const Reference<BinaryMultiplication> obj) const
			{
				return BinaryMultiplicationToString(obj, Indent);
			}
			std::string operator () (const Reference<BinaryDivision> obj) const
			{
				return BinaryDivisionToString(obj, Indent);
			}
		};

		return std::visit(BinaryExprVisitor(indentLevel), obj->Operation);
	}

	std::string ExpressionToString(const Reference<Expression> obj, size_t indentLevel)
	{
		struct ExpressionVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<TermExpr> obj) const
			{
				return TermExprToString(obj, Indent);
			}
			std::string operator () (const Reference<BinaryExpr> obj) const
			{
				return BinaryExprToString(obj, Indent);
			}
		};

		return std::visit(ExpressionVisitor(indentLevel), obj->Expr);
	}

}
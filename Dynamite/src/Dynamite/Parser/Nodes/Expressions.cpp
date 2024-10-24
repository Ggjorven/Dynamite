#include "dypch.h"
#include "Expressions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

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

	BinaryExpr::BinaryExpr(TokenType operation, const Type& resultType, Reference<Expression> lhs, Reference<Expression> rhs)
	{
		if (!Utils::IsEqual(operation, GetAllTokenTypeOperators()))
		{
			DY_LOG_FATAL("Tried to construct BinaryExpr from TokenType::{0}, but TokenType::{0} is not a binary operation.", Pulse::Enum::Name(operation));
			return;
		}

		// Switch on operation
		switch (operation)
		{
		case TokenType::Add:
		{
			Operation = Node::New<BinaryAddition>(resultType, lhs, rhs);
			break;
		}
		case TokenType::Subtract:
		{
			Operation = Node::New<BinarySubtraction>(resultType, lhs, rhs);
			break;
		}
		case TokenType::Multiply:
		{
			Operation = Node::New<BinaryMultiplication>(resultType, lhs, rhs);
			break;
		}
		case TokenType::Divide:
		{
			Operation = Node::New<BinaryDivision>(resultType, lhs, rhs);
			break;
		}

		// TODO: Other operations

		default:
			break;
		}
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
				std::string str(Indent, '\t');

				std::string termStr = TermExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] - '{0}')", Utils::RemoveFrontIndentation(termStr));

				return str;
			}
			std::string operator () (const Reference<BinaryExpr> obj) const
			{
				std::string str(Indent, '\t');
				
				std::string binaryStr = BinaryExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] - '{0}')", Utils::RemoveFrontIndentation(binaryStr));
				
				return str;
			}
		};

		return std::visit(ExpressionVisitor(indentLevel), obj->Expr);
	}

}
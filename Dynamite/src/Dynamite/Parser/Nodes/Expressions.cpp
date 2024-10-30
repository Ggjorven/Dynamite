#include "dypch.h"
#include "Expressions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Parser/Nodes/Functions.hpp"

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

		case TokenType::Or:
		{
			Operation = Node::New<BinaryOR>(resultType, lhs, rhs);
			break;
		}
		case TokenType::And:
		{
			Operation = Node::New<BinaryAND>(resultType, lhs, rhs);
			break;
		}
		case TokenType::Xor:
		{
			Operation = Node::New<BinaryXOR>(resultType, lhs, rhs);
			break;
		}

		default:
			break;
		}
	}

	AddressExpr::AddressExpr(Reference<Expression> expression)
		: Expr(expression)
	{
	}

	DereferenceExpr::DereferenceExpr(Reference<Expression> expression)
		: Expr(expression)
	{
	}

	Expression::Expression(VariantType expr)
		: Expr(expr)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type TermExpr::GetType() const
	{
		struct TermVisitor
		{
			Type operator () (const Reference<LiteralTerm> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<IdentifierTerm> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<ParenthesisTerm> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	bool TermExpr::IsLValue() const
	{
		struct TermVisitor
		{
			bool operator () (const Reference<LiteralTerm>) const
			{
				return false;
			}
			bool operator () (const Reference<IdentifierTerm>) const
			{
				return true;
			}
			bool operator () (const Reference<ParenthesisTerm> obj) const
			{
				return obj->IsLValue();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	Type BinaryExpr::GetType() const
	{
		struct OperationVisitor
		{
			Type operator () (const Reference<BinaryAddition> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<BinarySubtraction> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<BinaryMultiplication> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<BinaryDivision> obj) const
			{
				return obj->GetType();
			}

			Type operator () (const Reference<BinaryOR> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<BinaryAND> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<BinaryXOR> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(OperationVisitor(), Operation);
	}

	Type AddressExpr::GetType() const
	{
		Type ptrType = Expr->GetType().Copy();
		ptrType.AddPointer();

		return ptrType;
	}

	Type DereferenceExpr::GetType() const
	{
		Type noPtrType = Expr->GetType().Copy();
		if (!noPtrType.IsPointer())
		{
			DY_LOG_WARN("Trying to get the dereferenced type of a non-pointer type.");
			return noPtrType;
		}

		noPtrType.BackQualifiers.pop_back();

		return noPtrType;
	}

	Type Expression::GetType() const
	{
		struct ExpressionVisitor
		{
			Type operator () (const Reference<TermExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<BinaryExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<FunctionCall> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<AddressExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Reference<DereferenceExpr> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(ExpressionVisitor(), Expr);
	}

	bool Expression::IsLValue() const
	{
		struct ExpressionVisitor
		{
			bool operator () (const Reference<TermExpr> obj) const
			{
				return obj->IsLValue();
			}
			bool operator () (const Reference<BinaryExpr>) const
			{
				return false;
			}
			bool operator () (const Reference<FunctionCall> obj) const
			{
				return obj->GetType().IsPointer();
			}
			bool operator () (const Reference<AddressExpr> obj) const
			{
				return true;
			}
			bool operator () (const Reference<DereferenceExpr> obj) const
			{
				return true;
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
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string literalStr = LiteralTermToString(obj, Indent + 1);

				str += Pulse::Text::Format("([TermExpr] = '\n{0}'\n{1})", literalStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<IdentifierTerm> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string identifierStr = IdentifierTermToString(obj, Indent + 1);

				str += Pulse::Text::Format("([TermExpr] = '\n{0}'\n{1})", identifierStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<ParenthesisTerm> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string parenthesisStr = ParenthesisTermToString(obj, Indent + 1);

				str += Pulse::Text::Format("([TermExpr] = '\n{0}'\n{1})", parenthesisStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
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
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryAdditionToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<BinarySubtraction> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinarySubtractionToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<BinaryMultiplication> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryMultiplicationToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<BinaryDivision> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryDivisionToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}

			std::string operator () (const Reference<BinaryOR> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryORToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<BinaryAND> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryANDToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<BinaryXOR> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryXORToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
		};

		return std::visit(BinaryExprVisitor(indentLevel), obj->Operation);
	}

	std::string AddressExprToString(const Reference<AddressExpr> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([AddressExpr] = '\n{0}'\n{1})", exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string DereferenceExprToString(const Reference<DereferenceExpr> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([DereferenceExpr] = '\n{0}'\n{1})", exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string ExpressionToString(const Reference<Expression> obj, size_t indentLevel)
	{
		struct ExpressionVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<TermExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string termStr = TermExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", termStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<BinaryExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);
				
				std::string binaryStr = BinaryExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));
				
				return str;
			}
			std::string operator () (const Reference<FunctionCall> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string functionCall = FunctionCallToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", functionCall, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<AddressExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string addressStr = AddressExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", addressStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Reference<DereferenceExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string dereferenceStr = DereferenceExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", dereferenceStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
		};

		return std::visit(ExpressionVisitor(indentLevel), obj->Expr);
	}

}
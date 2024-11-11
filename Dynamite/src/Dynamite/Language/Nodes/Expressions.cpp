#include "dypch.h"
#include "Expressions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Language/Nodes/Booleans.hpp"
#include "Dynamite/Language/Nodes/Functions.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language::Node
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

	BinaryExpr::BinaryExpr(OperationType operation, const Type& resultType, Ref<Expression> lhs, Ref<Expression> rhs)
	{
		// Switch on operation
		switch (operation)
		{
		case OperationType::Add:
		{
			Operation = Node::New<BinaryAddition>(resultType, lhs, rhs);
			break;
		}
		case OperationType::Subtract:
		{
			Operation = Node::New<BinarySubtraction>(resultType, lhs, rhs);
			break;
		}
		case OperationType::Multiply:
		{
			Operation = Node::New<BinaryMultiplication>(resultType, lhs, rhs);
			break;
		}
		case OperationType::Divide:
		{
			Operation = Node::New<BinaryDivision>(resultType, lhs, rhs);
			break;
		}

		case OperationType::OR:
		{
			Operation = Node::New<BinaryOR>(resultType, lhs, rhs);
			break;
		}
		case OperationType::AND:
		{
			Operation = Node::New<BinaryAND>(resultType, lhs, rhs);
			break;
		}
		case OperationType::XOR:
		{
			Operation = Node::New<BinaryXOR>(resultType, lhs, rhs);
			break;
		}

		default:
			break;
		}
	}

	ReferenceExpr::ReferenceExpr(Ref<Expression> expr)
		: Expr(expr)
	{
	}

	AddressExpr::AddressExpr(Ref<Expression> expr)
		: Expr(expr)
	{
	}

	DereferenceExpr::DereferenceExpr(Ref<Expression> expr)
		: Expr(expr)
	{
	}

	CastExpr::CastExpr(Ref<Expression> expr, const Type& type)
		: Expr(expr), ToType(type)
	{
	}

	BooleanExpr::BooleanExpr(VariantType boolean)
		: Boolean(boolean)
	{
	}

	ArrayAccessExpr::ArrayAccessExpr(Ref<Expression> arrayExpr, Ref<Expression> indexExpr)
		: Array(arrayExpr), Index(indexExpr)
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
			Type operator () (const Ref<LiteralTerm> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<IdentifierTerm> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<ParenthesisTerm> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	NodeType TermExpr::GetUnderlyingType() const
	{
		struct TermVisitor
		{
			NodeType operator () (const Ref<LiteralTerm> obj) const
			{
				return NodeType::LiteralTerm;
			}
			NodeType operator () (const Ref<IdentifierTerm> obj) const
			{
				return NodeType::IdentifierTerm;
			}
			NodeType operator () (const Ref<ParenthesisTerm> obj) const
			{
				return obj->GetUnderlyingType();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	bool TermExpr::IsLValue() const
	{
		struct TermVisitor
		{
			bool operator () (const Ref<LiteralTerm>) const
			{
				return false;
			}
			bool operator () (const Ref<IdentifierTerm>) const
			{
				// Note: This is false, since when an identifier is passed it's passed by value.
				return false; 
			}
			bool operator () (const Ref<ParenthesisTerm> obj) const
			{
				return obj->IsLValue();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	Ref<Base> TermExpr::GetUnderlying()
	{
		struct TermVisitor
		{
			Ref<Base> operator () (Ref<LiteralTerm> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<IdentifierTerm> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<ParenthesisTerm> obj) const
			{
				return obj->GetUnderlying();
			}
		};

		return std::visit(TermVisitor(), Term);
	}

	Type BinaryExpr::GetType() const
	{
		struct OperationVisitor
		{
			Type operator () (const Ref<BinaryAddition> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BinarySubtraction> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BinaryMultiplication> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BinaryDivision> obj) const
			{
				return obj->GetType();
			}

			Type operator () (const Ref<BinaryOR> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BinaryAND> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BinaryXOR> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(OperationVisitor(), Operation);
	}

	NodeType BinaryExpr::GetUnderlyingType() const
	{
		struct OperationVisitor
		{
			NodeType operator () (const Ref<BinaryAddition>) const
			{
				return NodeType::BinaryAddition;
			}
			NodeType operator () (const Ref<BinarySubtraction>) const
			{
				return NodeType::BinarySubtraction;
			}
			NodeType operator () (const Ref<BinaryMultiplication>) const
			{
				return NodeType::BinaryMultiplication;
			}
			NodeType operator () (const Ref<BinaryDivision>) const
			{
				return NodeType::BinaryDivision;
			}

			NodeType operator () (const Ref<BinaryOR>) const
			{
				return NodeType::BinaryOR;
			}
			NodeType operator () (const Ref<BinaryAND>) const
			{
				return NodeType::BinaryAND;
			}
			NodeType operator () (const Ref<BinaryXOR>) const
			{
				return NodeType::BinaryXOR;
			}
		};

		return std::visit(OperationVisitor(), Operation);
	}

	Ref<Base> BinaryExpr::GetUnderlying()
	{
		struct OperationVisitor
		{
			Ref<Base> operator () (Ref<BinaryAddition> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<BinarySubtraction> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<BinaryMultiplication> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<BinaryDivision> obj) const
			{
				return (Ref<Base>)obj;
			}

			Ref<Base> operator () (Ref<BinaryOR> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<BinaryAND> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<BinaryXOR> obj) const
			{
				return (Ref<Base>)obj;
			}
		};

		return std::visit(OperationVisitor(), Operation);
	}

	Type ReferenceExpr::GetType() const
	{
		Type ptrType = Expr->GetType();
		ptrType.AddToBack(TypeQualifier::Reference);

		return ptrType;
	}

	NodeType ReferenceExpr::GetUnderlyingType() const
	{
		return Expr->GetUnderlyingType();
	}

	Ref<Base> ReferenceExpr::GetUnderlying()
	{
		return Expr->GetUnderlying();
	}

	Type AddressExpr::GetType() const
	{
		Type ptrType = Expr->GetType();
		ptrType.AddToBack(TypeQualifier::Pointer);

		return ptrType;
	}

	NodeType AddressExpr::GetUnderlyingType() const
	{
		return Expr->GetUnderlyingType();
	}

	Ref<Base> AddressExpr::GetUnderlying()
	{
		return Expr->GetUnderlying();
	}

	Type DereferenceExpr::GetType() const
	{
		Type noPtrType = Expr->GetType();
		if (!noPtrType.IsPointer() && !noPtrType.IsReference())
		{
			DY_LOG_WARN("Trying to get the dereferenced type of a non-pointer && non-reference type.");
			return noPtrType;
		}

		// Pointer
		if (noPtrType.IsPointer())
		{
			noPtrType.BackQualifiers.pop_back();
			noPtrType.BackQualifiers.emplace_back(TypeQualifier::Reference);
		}
		// Reference
		else
		{
			noPtrType.BackQualifiers.pop_back();
		}

		return noPtrType;
	}

	NodeType DereferenceExpr::GetUnderlyingType() const
	{
		return Expr->GetUnderlyingType();
	}

	Ref<Base> DereferenceExpr::GetUnderlying()
	{
		return Expr->GetUnderlying();
	}

	Type CastExpr::GetType() const
	{
		return ToType;
	}

	NodeType CastExpr::GetUnderlyingType() const
	{
		return Expr->GetUnderlyingType();
	}

	Ref<Base> CastExpr::GetUnderlying()
	{
		return Expr->GetUnderlying();
	}

	Type BooleanExpr::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType BooleanExpr::GetUnderlyingType() const
	{
		struct BooleanVisitor
		{
			NodeType operator () (const Ref<AndAndBoolean>) const
			{
				return NodeType::AndAndBoolean;
			}
			NodeType operator () (const Ref<OrOrBoolean>) const
			{
				return NodeType::OrOrBoolean;
			}
			NodeType operator () (const Ref<IsEqualBoolean>) const
			{
				return NodeType::IsEqualBoolean;
			}
			NodeType operator () (const Ref<MoreThanBoolean>) const
			{
				return NodeType::MoreThanBoolean;
			}
			NodeType operator () (const Ref<LessThanBoolean>) const
			{
				return NodeType::LessThanBoolean;
			}
		};

		return std::visit(BooleanVisitor(), Boolean);
	}

	Ref<Base> BooleanExpr::GetUnderlying()
	{
		struct BooleanVisitor
		{
			Ref<Base> operator () (Ref<AndAndBoolean> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<OrOrBoolean> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<IsEqualBoolean> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<MoreThanBoolean> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<LessThanBoolean> obj) const
			{
				return (Ref<Base>)obj;
			}
		};

		return std::visit(BooleanVisitor(), Boolean);
	}

	Type ArrayAccessExpr::GetType() const
	{
		// TODO: Check if should be reference?
		Type nonArrayType = Array->GetType();
		nonArrayType.PopBack();

		return nonArrayType;
	}

	NodeType ArrayAccessExpr::GetUnderlyingType() const
	{
		return NodeType::ArrayAccessExpr;
	}

	Ref<Base> ArrayAccessExpr::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type Expression::GetType() const
	{
		struct ExpressionVisitor
		{
			Type operator () (const Ref<TermExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BinaryExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<FunctionCall> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<ReferenceExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<AddressExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<DereferenceExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<CastExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<BooleanExpr> obj) const
			{
				return obj->GetType();
			}
			Type operator () (const Ref<ArrayAccessExpr> obj) const
			{
				return obj->GetType();
			}
		};

		return std::visit(ExpressionVisitor(), Expr);
	}

	NodeType Expression::GetUnderlyingType() const
	{
		struct ExpressionVisitor
		{
			NodeType operator () (const Ref<TermExpr> obj) const
			{
				return obj->GetUnderlyingType();
			}
			NodeType operator () (const Ref<BinaryExpr> obj) const
			{
				return obj->GetUnderlyingType();
			}
			NodeType operator () (const Ref<FunctionCall>) const
			{
				return NodeType::FunctionCall;
			}
			NodeType operator () (const Ref<ReferenceExpr>) const
			{
				return NodeType::ReferenceExpr;
			}
			NodeType operator () (const Ref<AddressExpr>) const
			{
				return NodeType::AddressExpr;
			}
			NodeType operator () (const Ref<DereferenceExpr>) const
			{
				return NodeType::DereferenceExpr;
			}
			NodeType operator () (const Ref<CastExpr>) const
			{
				return NodeType::CastExpr;
			}
			NodeType operator () (const Ref<BooleanExpr> obj) const
			{
				return obj->GetUnderlyingType();
			}
			NodeType operator () (const Ref<ArrayAccessExpr>) const
			{
				return NodeType::ArrayAccessExpr;
			}
		};

		return std::visit(ExpressionVisitor(), Expr);
	}

	bool Expression::IsLValue() const
	{
		struct ExpressionVisitor
		{
			bool operator () (const Ref<TermExpr> obj) const
			{
				return obj->IsLValue();
			}
			bool operator () (const Ref<BinaryExpr>) const
			{
				return false;
			}
			bool operator () (const Ref<FunctionCall> obj) const
			{
				return obj->GetType().IsPointer() || obj->GetType().IsReference();
			}
			bool operator () (const Ref<ReferenceExpr>) const
			{
				return true;
			}
			bool operator () (const Ref<AddressExpr>) const
			{
				return true;
			}
			bool operator () (const Ref<DereferenceExpr> obj) const
			{
				return obj->GetType().IsPointer() || obj->GetType().IsReference();
			}
			bool operator () (const Ref<CastExpr> obj) const
			{
				return obj->GetType().IsPointer() || obj->GetType().IsReference();
			}
			bool operator () (const Ref<BooleanExpr>) const
			{
				return false;
			}
			bool operator () (const Ref<ArrayAccessExpr> obj) const
			{
				return obj->GetType().IsPointer() || obj->GetType().IsReference();
			}
		};

		return std::visit(ExpressionVisitor(), Expr);
	}

	Ref<Base> Expression::GetUnderlying()
	{
		struct ExpressionVisitor
		{
			Ref<Base> operator () (Ref<TermExpr> obj) const
			{
				return obj->GetUnderlying();
			}
			Ref<Base> operator () (Ref<BinaryExpr> obj) const
			{
				return obj->GetUnderlying();
			}
			Ref<Base> operator () (Ref<FunctionCall> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<ReferenceExpr> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<AddressExpr> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<DereferenceExpr> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<CastExpr> obj) const
			{
				return (Ref<Base>)obj;
			}
			Ref<Base> operator () (Ref<BooleanExpr> obj) const
			{
				return obj->GetUnderlying();
			}
			
			Ref<Base> operator () (Ref<ArrayAccessExpr> obj) const
			{
				return (Ref<Base>)obj;
			}
		};

		return std::visit(ExpressionVisitor(), Expr);
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TermExprToString(const Ref<TermExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<TermExpr>)NullRef)
			return {};

		struct TermExprVisitor
		{
			size_t Indent;

			std::string operator () (const Ref<LiteralTerm> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string literalStr = LiteralTermToString(obj, Indent + 1);

				str += Pulse::Text::Format("([TermExpr] = '\n{0}'\n{1})", literalStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<IdentifierTerm> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string identifierStr = IdentifierTermToString(obj, Indent + 1);

				str += Pulse::Text::Format("([TermExpr] = '\n{0}'\n{1})", identifierStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<ParenthesisTerm> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string parenthesisStr = ParenthesisTermToString(obj, Indent + 1);

				str += Pulse::Text::Format("([TermExpr] = '\n{0}'\n{1})", parenthesisStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
		};

		return std::visit(TermExprVisitor(indentLevel), obj->Term);
	}

	std::string BinaryExprToString(const Ref<BinaryExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<BinaryExpr>)NullRef)
			return {};

		struct BinaryExprVisitor
		{
			size_t Indent;

			std::string operator () (const Ref<BinaryAddition> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryAdditionToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BinarySubtraction> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinarySubtractionToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BinaryMultiplication> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryMultiplicationToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BinaryDivision> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryDivisionToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}

			std::string operator () (const Ref<BinaryOR> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryORToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BinaryAND> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryANDToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BinaryXOR> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string binaryStr = BinaryXORToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BinaryExpr] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
		};

		return std::visit(BinaryExprVisitor(indentLevel), obj->Operation);
	}

	std::string ReferenceExprToString(const Ref<ReferenceExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<ReferenceExpr>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([ReferenceExpr] = '\n{0}'\n{1})", exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string AddressExprToString(const Ref<AddressExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<AddressExpr>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([AddressExpr] = '\n{0}'\n{1})", exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string DereferenceExprToString(const Ref<DereferenceExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<DereferenceExpr>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([DereferenceExpr] = '\n{0}'\n{1})", exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string CastExprToString(const Ref<CastExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<CastExpr>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([CastExpr{0}] = '\n{1}'\n{2})", TypeCollection::ToString(obj->GetType()), exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BooleanExprToString(const Ref<BooleanExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<BooleanExpr>)NullRef)
			return {};

		struct BooleanExprVisitor
		{
			size_t Indent;

			std::string operator () (const Ref<AndAndBoolean> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string andAndStr = AndAndBooleanToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BooleanExpr] = '\n{0}'\n{1})", andAndStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<OrOrBoolean> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string orOrStr = OrOrBooleanToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BooleanExpr] = '\n{0}'\n{1})", orOrStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<IsEqualBoolean> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string equalStr = IsEqualBooleanToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BooleanExpr] = '\n{0}'\n{1})", equalStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<MoreThanBoolean> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string moreStr = MoreThanBooleanToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BooleanExpr] = '\n{0}'\n{1})", moreStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<LessThanBoolean> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string lessStr = LessThanBooleanToString(obj, Indent + 1);

				str += Pulse::Text::Format("([BooleanExpr] = '\n{0}'\n{1})", lessStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
		};

		return std::visit(BooleanExprVisitor(indentLevel), obj->Boolean);
	}

	std::string ArrayAccessExprToString(const Ref<ArrayAccessExpr> obj, size_t indentLevel)
	{
		if (obj == (Ref<ArrayAccessExpr>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string arrayStr = ExpressionToString(obj->Array, indentLevel + 1);
		std::string indexStr = ExpressionToString(obj->Index, indentLevel + 1);

		str += Pulse::Text::Format("([ArrayAccessExpr{0}] = '\n{1}\n{3}Is being accessed with:\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), arrayStr, indexStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string ExpressionToString(const Ref<Expression> obj, size_t indentLevel)
	{
		if (obj == (Ref<Expression>)NullRef)
			return {};

		struct ExpressionVisitor
		{
			size_t Indent;

			std::string operator () (const Ref<TermExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string termStr = TermExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", termStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BinaryExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);
				
				std::string binaryStr = BinaryExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", binaryStr, Utils::StrTimes(Node::TabString, Indent));
				
				return str;
			}
			std::string operator () (const Ref<FunctionCall> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string functionCall = FunctionCallToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", functionCall, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<ReferenceExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string referenceStr = ReferenceExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", referenceStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<AddressExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string addressStr = AddressExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", addressStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<DereferenceExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string dereferenceStr = DereferenceExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", dereferenceStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<CastExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string castStr = CastExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", castStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<BooleanExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string boolStr = BooleanExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", boolStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
			std::string operator () (const Ref<ArrayAccessExpr> obj) const
			{
				std::string str = Utils::StrTimes(Node::TabString, Indent);

				std::string arrayStr = ArrayAccessExprToString(obj, Indent + 1);

				str += Pulse::Text::Format("([Expression] = '\n{0}'\n{1})", arrayStr, Utils::StrTimes(Node::TabString, Indent));

				return str;
			}
		};

		return std::visit(ExpressionVisitor(indentLevel), obj->Expr);
	}

}
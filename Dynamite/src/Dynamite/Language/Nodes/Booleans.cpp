#include "dypch.h"
#include "Booleans.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Language/Nodes/Expressions.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

namespace Dynamite::Language::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	AndAndBoolean::AndAndBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	OrOrBoolean::OrOrBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	IsEqualBoolean::IsEqualBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	NotBoolean::NotBoolean(Ref<Expression> expr)
		: Expr(expr)
	{
	}

	MoreThanBoolean::MoreThanBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	LessThanBoolean::LessThanBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	MoreThanOrEqualsBoolean::MoreThanOrEqualsBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	LessThanOrEqualsBoolean::LessThanOrEqualsBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type AndAndBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType AndAndBoolean::GetUnderlyingType() const
	{
		return NodeType::AndAndBoolean;
	}

	Ref<Base> AndAndBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type OrOrBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType OrOrBoolean::GetUnderlyingType() const
	{
		return NodeType::OrOrBoolean;
	}

	Ref<Base> OrOrBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type IsEqualBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType IsEqualBoolean::GetUnderlyingType() const
	{
		return NodeType::IsEqualBoolean;
	}

	Ref<Base> IsEqualBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type NotBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType NotBoolean::GetUnderlyingType() const
	{
		return NodeType::NotBoolean;
	}

	Ref<Base> NotBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type MoreThanBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType MoreThanBoolean::GetUnderlyingType() const
	{
		return NodeType::MoreThanBoolean;
	}

	Ref<Base> MoreThanBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type LessThanBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType LessThanBoolean::GetUnderlyingType() const
	{
		return NodeType::LessThanBoolean;
	}

	Ref<Base> LessThanBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type MoreThanOrEqualsBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType MoreThanOrEqualsBoolean::GetUnderlyingType() const
	{
		return NodeType::MoreThanOrEqualsBoolean;
	}

	Ref<Base> MoreThanOrEqualsBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	Type LessThanOrEqualsBoolean::GetType() const
	{
		return Type(TypeSpecifier::Bool);
	}

	NodeType LessThanOrEqualsBoolean::GetUnderlyingType() const
	{
		return NodeType::LessThanOrEqualsBoolean;
	}

	Ref<Base> LessThanOrEqualsBoolean::GetUnderlying()
	{
		return (Ref<Base>)this;
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string AndAndBooleanToString(const Ref<AndAndBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<AndAndBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([AndAndBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string OrOrBooleanToString(const Ref<OrOrBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<OrOrBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([OrOrBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string IsEqualBooleanToString(const Ref<IsEqualBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<IsEqualBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([IsEqualBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string NotBooleanToString(const Ref<NotBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<NotBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([NotBoolean] = '\n{0}'\n{1})", exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string MoreThanBooleanToString(const Ref<MoreThanBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<MoreThanBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([MoreThanBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string LessThanBooleanToString(const Ref<LessThanBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<LessThanBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([LessThanBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string MoreThanOrEqualsBooleanToString(const Ref<MoreThanOrEqualsBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<MoreThanOrEqualsBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([MoreThanOrEqualsBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string LessThanOrEqualsBooleanToString(const Ref<LessThanOrEqualsBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<LessThanOrEqualsBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([LessThanOrEqualsBoolean] = '\n{0}\n{1}'\n{2})", lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

}
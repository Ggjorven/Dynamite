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

	MoreThanBoolean::MoreThanBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	LessThanBoolean::LessThanBoolean(Ref<Expression> lhs, Ref<Expression> rhs)
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

	/////////////////////////////////////////////////////////////////
	// Helper functions // TODO: Adjust to BinaryExpr format. LHS & RHS
	/////////////////////////////////////////////////////////////////
	std::string AndAndBooleanToString(const Ref<AndAndBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<AndAndBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsStr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsStr = ExpressionToString(obj->RHS, indentLevel + 1);

		str += Pulse::Text::Format("([AndAndBoolean{0}] = '\n{1}\n{3}&&\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhsStr, rhsStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string OrOrBooleanToString(const Ref<OrOrBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<OrOrBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsStr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsStr = ExpressionToString(obj->RHS, indentLevel + 1);

		str += Pulse::Text::Format("([OrOrBoolean{0}] = '\n{1}\n{3}||\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhsStr, rhsStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string IsEqualBooleanToString(const Ref<IsEqualBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<IsEqualBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsStr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsStr = ExpressionToString(obj->RHS, indentLevel + 1);

		str += Pulse::Text::Format("([IsEqualBoolean{0}] = '\n{1}\n{3}==\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhsStr, rhsStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string MoreThanBooleanToString(const Ref<MoreThanBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<MoreThanBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsStr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsStr = ExpressionToString(obj->RHS, indentLevel + 1);

		str += Pulse::Text::Format("([MoreThanBoolean{0}] = '\n{1}\n{3}>\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhsStr, rhsStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string LessThanBooleanToString(const Ref<LessThanBoolean> obj, size_t indentLevel)
	{
		if (obj == (Ref<LessThanBoolean>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsStr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsStr = ExpressionToString(obj->RHS, indentLevel + 1);

		str += Pulse::Text::Format("([LessThanBoolean{0}] = '\n{1}\n{3}<\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhsStr, rhsStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

}
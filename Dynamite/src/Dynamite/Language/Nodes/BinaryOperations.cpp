#include "dypch.h"
#include "BinaryOperations.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

#include "Dynamite/Language/Nodes/Expressions.hpp"

namespace Dynamite::Language::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	BinaryAddition::BinaryAddition(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinarySubtraction::BinarySubtraction(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryMultiplication::BinaryMultiplication(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryDivision::BinaryDivision(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryOR::BinaryOR(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryAND::BinaryAND(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryXOR::BinaryXOR(const Type& type, Ref<Expression> lhs, Ref<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type BinaryAddition::GetType() const
	{
		return ResultType;
	}

	Type BinarySubtraction::GetType() const
	{
		return ResultType;
	}

	Type BinaryMultiplication::GetType() const
	{
		return ResultType;
	}

	Type BinaryDivision::GetType() const
	{
		return ResultType;
	}

	Type BinaryOR::GetType() const
	{
		return ResultType;
	}

	Type BinaryAND::GetType() const
	{
		return ResultType;
	}

	Type BinaryXOR::GetType() const
	{
		return ResultType;
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string BinaryAdditionToString(const Ref<BinaryAddition> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryAddition({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinarySubtractionToString(const Ref<BinarySubtraction> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinarySubtraction({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryMultiplicationToString(const Ref<BinaryMultiplication> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryMultiplication({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryDivisionToString(const Ref<BinaryDivision> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryDivision({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryORToString(const Ref<BinaryOR> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryOR({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryANDToString(const Ref<BinaryAND> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryAND({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryXORToString(const Ref<BinaryXOR> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryXOR({0})] = '\n{1}\n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

}
#include "dypch.h"
#include "BinaryOperations.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Parser/Nodes/Expressions.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	BinaryAddition::BinaryAddition(const Type& type, Reference<Expression> lhs, Reference<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinarySubtraction::BinarySubtraction(const Type& type, Reference<Expression> lhs, Reference<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryMultiplication::BinaryMultiplication(const Type& type, Reference<Expression> lhs, Reference<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	BinaryDivision::BinaryDivision(const Type& type, Reference<Expression> lhs, Reference<Expression> rhs)
		: ResultType(type), LHS(lhs), RHS(rhs)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type& BinaryAddition::GetType()
	{
		return ResultType;
	}

	Type& BinarySubtraction::GetType()
	{
		return ResultType;
	}

	Type& BinaryMultiplication::GetType()
	{
		return ResultType;
	}

	Type& BinaryDivision::GetType()
	{
		return ResultType;
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string BinaryAdditionToString(const Reference<BinaryAddition> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryAddition({0})] = '\n{1}\n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinarySubtractionToString(const Reference<BinarySubtraction> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinarySubtraction({0})] = '\n{1}\n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryMultiplicationToString(const Reference<BinaryMultiplication> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryMultiplication({0})] = '\n{1}\n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string BinaryDivisionToString(const Reference<BinaryDivision> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string lhsExpr = ExpressionToString(obj->LHS, indentLevel + 1);
		std::string rhsExpr = ExpressionToString(obj->RHS, indentLevel + 1);
		std::string lhs = Utils::EmplaceAfterIndentation(lhsExpr, "LHS: ");
		std::string rhs = Utils::EmplaceAfterIndentation(rhsExpr, "RHS: ");

		str += Pulse::Text::Format("([BinaryDivision({0})] = '\n{1}\n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), lhs, rhs, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	Optional<size_t> GetBinaryPrecendence(TokenType operation)
	{
		switch (operation)
		{
		case TokenType::Add:
		case TokenType::Subtract:
			return 0;

		case TokenType::Multiply:
		case TokenType::Divide:
			return 1;

		// TODO: Additional binary operators

		default:
			break;
		}

		return {};
	}

}
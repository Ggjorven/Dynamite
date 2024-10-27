#include "dypch.h"
#include "Conditions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Parser/Nodes/Expressions.hpp"
#include "Dynamite/Parser/Nodes/Statements.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	ElseIfBranch::ElseIfBranch(Reference<Expression> expr, Reference<ScopeStatement> scope, Optional<Reference<ConditionBranch>> next)
		: Expr(expr), Scope(scope), Next(next)
	{
	}

	ElseBranch::ElseBranch(Reference<ScopeStatement> scope)
		: Scope(scope)
	{
	}

	ConditionBranch::ConditionBranch(VariantType branch)
		: Branch(branch)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string ElseIfBranchToString(const Reference<ElseIfBranch> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string expressionStr = ExpressionToString(obj->Expr, indentLevel + 1);
		std::string scopeStr = ScopeStatementToString(obj->Scope, indentLevel);

		if (obj->Next.HasValue())
		{
			std::string nextStr = ConditionBranchToString(obj->Next.Value(), indentLevel);
			str += Pulse::Text::Format("([ElseIfBranch] = '( \n{0} \n{2}) \n{1}'\n{2})\n{3}", expressionStr, scopeStr, Utils::StrTimes(Node::TabString, indentLevel), nextStr);
		}
		else
		{
			str += Pulse::Text::Format("([ElseIfBranch] = '( \n{0} \n{2}) \n{1}'\n{2})", expressionStr, scopeStr, Utils::StrTimes(Node::TabString, indentLevel));
		}

		return str;
	}

	std::string ElseBranchToString(const Reference<ElseBranch> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string scope = ScopeStatementToString(obj->Scope, indentLevel);

		str += Pulse::Text::Format("([ElseBranch])\n{0}", scope);

		return str;
	}

	std::string ConditionBranchToString(const Reference<ConditionBranch> obj, size_t indentLevel)
	{
		struct ConditionVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<ElseIfBranch> obj) const
			{
				return ElseIfBranchToString(obj, Indent);
			}

			std::string operator () (const Reference<ElseBranch> obj) const
			{
				return ElseBranchToString(obj, Indent);
			}
		};

		return std::visit(ConditionVisitor(indentLevel), obj->Branch);
	}

}
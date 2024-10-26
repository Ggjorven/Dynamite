#include "dypch.h"
#include "Statements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

#include "Dynamite/Parser/Nodes/Functions.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	IfStatement::IfStatement(Reference<Expression> expr, Reference<ScopeStatement> scope, Optional<Reference<ConditionBranch>> next)
		: Expr(expr), Scope(scope), Next(next)
	{
	}

	VariableStatement::VariableStatement(const Type& type, const Token& variable, Reference<Expression> expr)
		: VariableType(type), Variable(variable), Expr(expr)
	{
	}

	ScopeStatement::ScopeStatement(const std::vector<Reference<Statement>>& statements)
		: Statements(statements)
	{
	}

	AssignmentStatement::AssignmentStatement(const Type& type, const Token& variable, Reference<Expression> expr)
		: VariableType(type), Variable(variable), Expr(expr)
	{
	}

	ReturnStatement::ReturnStatement(Reference<Expression> expr)
		: Expr(expr)
	{
	}

	Statement::Statement(VariantType statement)
		: StatementObj(statement)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type& VariableStatement::GetType()
	{
		return VariableType;
	}


	Type& AssignmentStatement::GetType()
	{
		return VariableType;
	}

	Type& ReturnStatement::GetType()
	{
		return Expr->GetType();
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string IfStatementToString(const Reference<IfStatement> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string expressionStr = ExpressionToString(obj->Expr, indentLevel + 1);
		std::string scopeStr = ScopeStatementToString(obj->Scope, indentLevel);

		if (obj->Next.HasValue())
		{
			std::string nextStr = ConditionBranchToString(obj->Next.Value(), indentLevel);
			str += Pulse::Text::Format("([IfStatement] = '( \n{0} \n{2}) \n{1}'\n{2})\n{3}", expressionStr, scopeStr, Utils::StrTimes(Node::TabString, indentLevel), nextStr);
		}
		else
		{
			str += Pulse::Text::Format("([IfStatement] = '( \n{0} \n{2}) \n{1}'\n{2})", expressionStr, scopeStr, Utils::StrTimes(Node::TabString, indentLevel));
		}

		return str;
	}

	std::string VariableStatementToString(const Reference<VariableStatement> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		if (obj->Expr)
		{
			std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);
			str += Pulse::Text::Format("([VariableStatement] = '{0} {1} = \n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), obj->Variable.Value, exprStr, Utils::StrTimes(Node::TabString, indentLevel));
		}
		else
		{
			str += Pulse::Text::Format("([VariableStatement] = '{0} {1}')", TypeSystem::ToString(obj->GetType()), obj->Variable.Value);
		}

		return str;
	}

	std::string ScopeStatementToString(const Reference<ScopeStatement> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string statementsStr = {};
		for (size_t i = 0; i < obj->Statements.size(); i++)
		{
			if (i > 0)
				statementsStr += "\n\n";

			statementsStr += StatementToString(obj->Statements[i], indentLevel + 1);
		}

		str += Pulse::Text::Format("([ScopeStatement] = '{{\n{0}\n{1}}}'\n{2})", statementsStr, Utils::StrTimes(Node::TabString, indentLevel + 1), Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string AssignmentStatementToString(const Reference<AssignmentStatement> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);
		str += Pulse::Text::Format("([AssignmentStatement({0})] = '{1} = \n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), obj->Variable.Value, exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string ReturnStatementToString(const Reference<ReturnStatement> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string returnStr = ExpressionToString(obj->Expr, indentLevel + 1);
		str += Pulse::Text::Format("([ReturnStatement({0})] = '\n{1}'\n{2})", TypeSystem::ToString(obj->GetType()), returnStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string StatementToString(const Reference<Statement> obj, size_t indentLevel)
	{
		struct StatementVisitor
		{
			size_t Ident;

			std::string operator () (const Reference<IfStatement> obj) const
			{
				return IfStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<VariableStatement> obj) const
			{
				return VariableStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<ScopeStatement> obj) const
			{
				return ScopeStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<AssignmentStatement> obj) const
			{
				return AssignmentStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<ReturnStatement> obj) const
			{
				return ReturnStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<FunctionCall> obj) const
			{
				return FunctionCallToString(obj, Ident);
			}
		};

		return std::visit(StatementVisitor(indentLevel), obj->StatementObj);
	}

}
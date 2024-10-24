#include "dypch.h"
#include "Statements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

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

	AssignmentStatement::AssignmentStatement(const Token& variable, Reference<Expression> expr)
		: Variable(variable), Expr(expr)
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

	/////////////////////////////////////////////////////////////////
	// Helper functions // TODO: ...
	/////////////////////////////////////////////////////////////////
	std::string IfStatementToString(const Reference<IfStatement> obj, size_t indentLevel)
	{
		return std::string();
	}

	std::string VariableStatementToString(const Reference<VariableStatement> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);

		str += Pulse::Text::Format("([VariableStatement] = '{0} {1} = \n{2}'\n{3})", TypeSystem::ToString(obj->GetType()), obj->Variable.Value, exprStr, Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string ScopeStatementToString(const Reference<ScopeStatement> obj, size_t indentLevel)
	{
		return std::string();
	}

	std::string AssignmentStatementToString(const Reference<AssignmentStatement> obj, size_t indentLevel)
	{
		return std::string();
	}

	std::string StatementToString(const Reference<Statement> obj, size_t indentLevel)
	{
		struct StatementVisitor
		{
			size_t Ident;

			std::string operator () (const Reference<IfStatement> obj)
			{
				return IfStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<VariableStatement> obj)
			{
				return VariableStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<ScopeStatement> obj)
			{
				return ScopeStatementToString(obj, Ident);
			}
			std::string operator () (const Reference<AssignmentStatement> obj)
			{
				return AssignmentStatementToString(obj, Ident);
			}
		};

		return std::visit(StatementVisitor(indentLevel), obj->StatementObj);
	}

}
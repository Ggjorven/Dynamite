#include "dypch.h"
#include "Statements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Language/Nodes/Functions.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

namespace Dynamite::Language::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	IfStatement::IfStatement(Ref<Expression> expr, Ref<ScopeStatement> scope, Optional<Ref<ConditionBranch>> next)
		: Expr(expr), Scope(scope), Next(next)
	{
	}

	VariableStatement::VariableStatement(const Type& type, const std::string& variable, Ref<Expression> expr)
		: VariableType(type), Variable(variable), Expr(expr)
	{
	}

	ScopeStatement::ScopeStatement(const std::vector<Ref<Statement>>& statements)
		: Statements(statements)
	{
	}

	AssignmentStatement::AssignmentStatement(Ref<Expression> variable, Ref<Expression> expr)
		: Variable(variable), Expr(expr)
	{
	}

	ReturnStatement::ReturnStatement(Ref<Expression> expr)
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
	Type VariableStatement::GetType() const
	{
		return VariableType;
	}

	Type AssignmentStatement::GetType() const
	{
		return Variable->GetType();
	}

	Type ReturnStatement::GetType() const
	{
		if (Expr)
			return Expr->GetType();
		else
			return Type(TypeSpecifier::Void);
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string IfStatementToString(const Ref<IfStatement> obj, size_t indentLevel)
	{
		if (obj == (Ref<IfStatement>)NullRef)
			return {};

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

	std::string VariableStatementToString(const Ref<VariableStatement> obj, size_t indentLevel)
	{
		if (obj == (Ref<VariableStatement>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		if (obj->Expr)
		{
			std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);
			str += Pulse::Text::Format("([VariableStatement] = '{0} {1} = \n{2}'\n{3})", TypeCollection::ToString(obj->GetType()), obj->Variable, exprStr, Utils::StrTimes(Node::TabString, indentLevel));
		}
		else
		{
			str += Pulse::Text::Format("([VariableStatement] = '{0} {1}')", TypeCollection::ToString(obj->GetType()), obj->Variable);
		}

		return str;
	}

	std::string ScopeStatementToString(const Ref<ScopeStatement> obj, size_t indentLevel)
	{
		if (obj == (Ref<ScopeStatement>)NullRef)
			return {};

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

	std::string AssignmentStatementToString(const Ref<AssignmentStatement> obj, size_t indentLevel)
	{
		if (obj == (Ref<AssignmentStatement>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string varStr = ExpressionToString(obj->Variable, indentLevel + 1);
		std::string exprStr = ExpressionToString(obj->Expr, indentLevel + 1);
		str += Pulse::Text::Format("([AssignmentStatement({0})] = '\n{1}\n{2}=\n{3}'\n{2})", TypeCollection::ToString(obj->GetType()), Utils::StrTimes(Node::TabString, indentLevel), varStr, exprStr);

		return str;
	}

	std::string ReturnStatementToString(const Ref<ReturnStatement> obj, size_t indentLevel)
	{
		if (obj == (Ref<ReturnStatement>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		if (obj->Expr)
		{
			std::string returnStr = ExpressionToString(obj->Expr, indentLevel + 1);
			str += Pulse::Text::Format("([ReturnStatement({0})] = '\n{1}'\n{2})", TypeCollection::ToString(obj->GetType()), returnStr, Utils::StrTimes(Node::TabString, indentLevel));
		}
		else
		{
			str += Pulse::Text::Format("([ReturnStatement({0})])", TypeCollection::ToString(obj->GetType()));
		}

		return str;
	}

	std::string StatementToString(const Ref<Statement> obj, size_t indentLevel)
	{
		if (obj == (Ref<Statement>)NullRef)
			return {};

		struct StatementVisitor
		{
			size_t Ident;

			std::string operator () (const Ref<IfStatement> obj) const
			{
				return IfStatementToString(obj, Ident);
			}
			std::string operator () (const Ref<VariableStatement> obj) const
			{
				return VariableStatementToString(obj, Ident);
			}
			std::string operator () (const Ref<ScopeStatement> obj) const
			{
				return ScopeStatementToString(obj, Ident);
			}
			std::string operator () (const Ref<AssignmentStatement> obj) const
			{
				return AssignmentStatementToString(obj, Ident);
			}
			std::string operator () (const Ref<ReturnStatement> obj) const
			{
				return ReturnStatementToString(obj, Ident);
			}
			std::string operator () (const Ref<FunctionCall> obj) const
			{
				return FunctionCallToString(obj, Ident);
			}
		};

		return std::visit(StatementVisitor(indentLevel), obj->StatementObj);
	}

}
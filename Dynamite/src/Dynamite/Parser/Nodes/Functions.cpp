#include "dypch.h"
#include "Functions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"

#include "Dynamite/Parser/Nodes/Statements.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	FunctionDeclaration::FunctionDeclaration(const Type& returnType, const Token& name, const std::vector<Reference<VariableStatement>>& parameters)
		: ReturnType(returnType), Name(name), Parameters(parameters)
	{
	}

	FunctionDefinition::FunctionDefinition(const Type& returnType, const Token& name, const std::vector<Reference<VariableStatement>>& parameters, Reference<ScopeStatement> body)
		: ReturnType(returnType), Name(name), Parameters(parameters), Body(body)
	{
	}

    Function::Function(VariantType function)
		: Func(function)
	{
	}

	FunctionCall::FunctionCall(const Type& returnType, const Token& function, const std::vector<Reference<Expression>>& arguments)
		: ReturnType(returnType), Function(function), Arguments(arguments)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type FunctionDeclaration::GetType() const
	{
		return ReturnType;
	}

	Type FunctionDefinition::GetType() const
	{
		return ReturnType;
	}

	Type Function::GetType() const
	{
		struct FunctionVisitor
		{
			Type operator () (const Reference<FunctionDeclaration> obj)
			{
				return obj->GetType();
			}
			Type operator () (const Reference<FunctionDefinition> obj)
			{
				return obj->GetType();
			}
		};

		return std::visit(FunctionVisitor(), Func);
	}

	Type FunctionCall::GetType() const
	{
		return ReturnType;
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string FunctionDeclarationToString(const Reference<FunctionDeclaration> obj, size_t indentLevel) // TODO: Check if correct
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string returnType = TypeSystem::ToString(obj->ReturnType);
		std::string functionName = obj->Name.Value;

		std::string parameters = {};
		for (size_t i = 0; i < obj->Parameters.size(); i++)
		{
			if (i > 0)
				parameters += ",\n";

			std::string parameterStr = VariableStatementToString(obj->Parameters[i], indentLevel + 2);
			parameters += parameterStr;
		}

		str += Pulse::Text::Format("([FunctionDeclaration] = '{0} {1}(\n{2}\n{3})'\n{4})", returnType, functionName, parameters, Utils::StrTimes(Node::TabString, indentLevel + 1), Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string FunctionDefinitionToString(const Reference<FunctionDefinition> obj, size_t indentLevel)
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string returnType = TypeSystem::ToString(obj->ReturnType);
		std::string functionName = obj->Name.Value;

		std::string parameters = {};
		for (size_t i = 0; i < obj->Parameters.size(); i++)
		{
			if (i > 0)
				parameters += ",\n";

			std::string parameterStr = VariableStatementToString(obj->Parameters[i], indentLevel + 2);
			parameters += parameterStr;
		}

		std::string scope = ScopeStatementToString(obj->Body, indentLevel + 1);

		str += Pulse::Text::Format("([FunctionDefinition] = '{0} {1}(\n{2}\n{4})\n{3}'\n{5})", returnType, functionName, parameters, scope, Utils::StrTimes(Node::TabString, indentLevel + 1), Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string FunctionCallToString(const Reference<FunctionCall> obj, size_t indentLevel) 
	{
		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		std::string returnType = TypeSystem::ToString(obj->ReturnType);
		std::string functionName = obj->Function.Value;

		std::string arguments = {};
		for (size_t i = 0; i < obj->Arguments.size(); i++)
		{
			if (i > 0)
				arguments += ",\n";

			std::string argumentStr = ExpressionToString(obj->Arguments[i], indentLevel + 2);
			arguments += argumentStr;
		}

		str += Pulse::Text::Format("([FunctionCall({0})] = '{1}(\n{2}\n{3})'\n{4})", returnType, functionName, arguments, Utils::StrTimes(Node::TabString, indentLevel + 1), Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

}
#include "dypch.h"
#include "All.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	Function::Function(const Type& returnType, const Token& name, const std::vector<Reference<VariableStatement>>& parameters, Reference<ScopeStatement> body)
		: ReturnType(returnType), Name(name), Parameters(parameters), Body(body)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////
	Type& Function::GetType()
	{
		return ReturnType;
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions 
	/////////////////////////////////////////////////////////////////
	std::string FunctionToString(const Reference<Function> obj, size_t indentLevel)
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

		str += Pulse::Text::Format("([Function] = '{0} {1}(\n{2}\n{4})\n{3}'\n{5})", returnType, functionName, parameters, scope, Utils::StrTimes(Node::TabString, indentLevel + 1), Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string DefineToString(const Program::VariantType& obj, size_t indentLevel)
	{
		struct DefineVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<Function> obj) const
			{
				return FunctionToString(obj, Indent);
			}
			std::string operator () (const Reference<VariableStatement> obj) const
			{
				return VariableStatementToString(obj, Indent);
			}
		};

		return std::visit(DefineVisitor(indentLevel), obj);
	}

}
#include "dypch.h"
#include "All.hpp"

#include "Dynamite/Core/Logging.hpp"

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
	// Helper functions // TODO: ...
	/////////////////////////////////////////////////////////////////
	std::string FunctionToString(const Reference<Function> obj, size_t indentLevel)
	{
		std::string str(indentLevel, '\t');

		str += Pulse::Text::Format("([Function] )");
		/*
		Type ReturnType;
		Token Name;

		std::vector<Reference<VariableStatement>> Parameters;

		Reference<ScopeStatement> Body;
		*/

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
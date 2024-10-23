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
		/*
		Type ReturnType;
		Token Name;

		std::vector<Reference<VariableStatement>> Parameters;

		Reference<ScopeStatement> Body;
		*/

		return std::string();
	}

}
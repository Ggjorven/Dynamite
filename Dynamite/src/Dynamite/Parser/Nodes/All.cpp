#include "dypch.h"
#include "All.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Helper functions 
	/////////////////////////////////////////////////////////////////
	std::string DefineToString(const Program::VariantType& obj, size_t indentLevel)
	{
		struct DefineVisitor
		{
			size_t Indent;

			std::string operator () (const Reference<FunctionDeclaration> obj) const
			{
				return FunctionDeclarationToString(obj, Indent);
			}
			std::string operator () (const Reference<FunctionDefinition> obj) const
			{
				return FunctionDefinitionToString(obj, Indent);
			}
			std::string operator () (const Reference<VariableStatement> obj) const
			{
				return VariableStatementToString(obj, Indent);
			}
		};

		return std::visit(DefineVisitor(indentLevel), obj);
	}

}
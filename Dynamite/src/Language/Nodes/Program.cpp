#include "dypch.h"
#include "Program.hpp"

#include "Core/Logging.hpp"

#include "Utils/Utils.hpp"

namespace Dynamite::Language::Node
{

	/////////////////////////////////////////////////////////////////
	// Helper functions 
	/////////////////////////////////////////////////////////////////
	std::string DefineToString(const Program::VariantType& obj, size_t indentLevel)
	{
		struct DefineVisitor
		{
			size_t Indent;

			std::string operator () (const Ref<FunctionDeclaration> obj) const
			{
				return FunctionDeclarationToString(obj, Indent);
			}
			std::string operator () (const Ref<FunctionDefinition> obj) const
			{
				return FunctionDefinitionToString(obj, Indent);
			}
			std::string operator () (const Ref<VariableStatement> obj) const
			{
				return VariableStatementToString(obj, Indent);
			}
		};

		return std::visit(DefineVisitor(indentLevel), obj);
	}

}
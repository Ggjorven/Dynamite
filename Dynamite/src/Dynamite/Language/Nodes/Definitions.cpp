#include "dypch.h"
#include "Definitions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Language/Nodes/Functions.hpp"
#include "Dynamite/Language/Nodes/Statements.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

namespace Dynamite::Language::Node
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
	std::string DefineToString(const Definition& obj, size_t indentLevel)
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
			std::string operator () (const Ref<NamespaceDefinition> obj) const
			{
				return {};
			}
			std::string operator () (const Ref<ClassDeclaration> obj) const
			{
				return {};
			}
			std::string operator () (const Ref<ClassDefinition> obj) const
			{
				return {};
			}
			std::string operator () (const Ref<StructDeclaration> obj) const
			{
				return {};
			}
			std::string operator () (const Ref<StructDefinition> obj) const
			{
				return {};
			}
			std::string operator () (const Ref<EnumDefinition> obj) const
			{
				return {};
			}
		};

		return std::visit(DefineVisitor(indentLevel), obj);
	}

}
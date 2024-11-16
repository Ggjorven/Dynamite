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
	NamespaceDefinition::NamespaceDefinition(const std::string& identifier, const std::vector<Definition>& definitions)
		: Identifier(identifier), Definitions(definitions)
	{
	}

	ClassDeclaration::ClassDeclaration(const std::string& identifier)
		: Identifier(identifier)
	{
	}

	ClassDefinition::ClassDefinition(const std::string& identifier, const Map<std::string, Helper::Member<VariableStatement>>& variables, const Map<std::string, Helper::Member<FunctionDefinition>>& functions)
		: Identifier(identifier), Variables(variables), Functions(functions)
	{
	}

	StructDeclaration::StructDeclaration(const std::string& identifier)
		: Identifier(identifier)
	{
	}

	StructDefinition::StructDefinition(const std::string& identifier, const Map<std::string, Helper::Member<VariableStatement>>& variables, const Map<std::string, Helper::Member<FunctionDefinition>>& functions)
		: Identifier(identifier), Variables(variables), Functions(functions)
	{
	}

	EnumDefinition::EnumDefinition(const std::string& identifier, const Type& integerType, const Map<std::string, std::string>& names)
		: Identifier(identifier), IntegerType(integerType), Names(names)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string NamespaceDefinitionToString(const Ref<NamespaceDefinition> obj, size_t indentLevel)
	{
		if (obj == (Ref<NamespaceDefinition>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);
		str += Pulse::Text::Format("([NamespaceDefinition({0})] = '", obj->Identifier);

		for (const auto& definition : obj->Definitions)
		{
			str += "\n\n" + DefinitionToString(definition, indentLevel + 1);
		}

		str += Pulse::Text::Format("'\n{0})", Utils::StrTimes(Node::TabString, indentLevel));

		return str;
	}

	std::string ClassDeclarationToString(const Ref<ClassDeclaration> obj, size_t indentLevel)
	{
		if (obj == (Ref<ClassDeclaration>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([ClassDeclaration({0})])", obj->Identifier);

		return str;
	}

	std::string ClassDefinitionToString(const Ref<ClassDefinition> obj, size_t indentLevel)
	{
		if (obj == (Ref<ClassDefinition>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([ClassDefinition({0})] = 'TODO')", obj->Identifier);

		return str;
	}

	std::string StructDeclarationToString(const Ref<StructDeclaration> obj, size_t indentLevel)
	{
		if (obj == (Ref<StructDeclaration>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([StructDeclaration({0})])", obj->Identifier);

		return str;
	}

	std::string StructDefinitionToString(const Ref<StructDefinition> obj, size_t indentLevel)
	{
		if (obj == (Ref<StructDefinition>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([StructDefinition({0})] = 'TODO')", obj->Identifier);

		return str;
	}

	std::string EnumDefinitionToString(const Ref<EnumDefinition> obj, size_t indentLevel)
	{
		if (obj == (Ref<EnumDefinition>)NullRef)
			return {};

		std::string str = Utils::StrTimes(Node::TabString, indentLevel);

		str += Pulse::Text::Format("([EnumDefinition({0})] = 'TODO')", obj->Identifier);

		return str;
	}

	std::string DefinitionToString(const Definition& obj, size_t indentLevel)
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
				return NamespaceDefinitionToString(obj, Indent);
			}
			std::string operator () (const Ref<ClassDeclaration> obj) const
			{
				return ClassDeclarationToString(obj, Indent);
			}
			std::string operator () (const Ref<ClassDefinition> obj) const
			{
				return ClassDefinitionToString(obj, Indent);
			}
			std::string operator () (const Ref<StructDeclaration> obj) const
			{
				return StructDeclarationToString(obj, Indent);
			}
			std::string operator () (const Ref<StructDefinition> obj) const
			{
				return StructDefinitionToString(obj, Indent);
			}
			std::string operator () (const Ref<EnumDefinition> obj) const
			{
				return EnumDefinitionToString(obj, Indent);
			}
		};

		return std::visit(DefineVisitor(indentLevel), obj);
	}

}
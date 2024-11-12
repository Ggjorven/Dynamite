#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/Utils/Namespace.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// ParserState
	/////////////////////////////////////////////////////////////////
	class ParserState
	{
	public:
		// Namespace
		inline void PushNamespace(const std::string& name) { s_Namespace.AddLevel(name); }
		inline void PopNamespace() { s_Namespace.PopLevels(1); }
		inline const Language::Namespace& GetCurrentNamespace() { return s_Namespace; }

		// Class
		inline void PushClass(const std::string& name) { s_CurrentClass = name; }
		inline void PopClass() { s_CurrentClass.clear(); }
		inline const std::string& GetCurrentClass() { return s_CurrentClass; }

		// Function definition
		inline void PushCurrentFunction(Language::Node::Ref<Language::Node::FunctionDefinition> func) { s_CurrentFunction = func; }
		inline void PopCurrentFunction() { s_CurrentFunction = (Language::Node::Ref<Language::Node::FunctionDefinition>)Language::Node::NullRef; }
		inline Language::Node::Ref<Language::Node::FunctionDefinition> GetCurrentFunction() { return s_CurrentFunction; }

	private:
		inline static Language::Namespace s_Namespace = {};
		inline static std::string s_CurrentClass = {};
		inline static Language::Node::Ref<Language::Node::FunctionDefinition> s_CurrentFunction = (Language::Node::Ref<Language::Node::FunctionDefinition>)Language::Node::NullRef;
	};

}
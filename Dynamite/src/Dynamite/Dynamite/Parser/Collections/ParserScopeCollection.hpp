#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/Utils/Namespace.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	struct ParserVariable
	{
	public:
		Language::Namespace Namespaces;
		std::string ClassName;

		std::string Name;
		Language::Type VariableType;
	};

	/////////////////////////////////////////////////////////////////
	// ParserScopeCollection
	/////////////////////////////////////////////////////////////////
	class ParserScopeCollection
	{
	public:
		static void Reset();

		static void BeginScope();
		static void EndScope();

		static void PushVar(const ParserVariable& variable);

		static bool Exists(const Language::Namespace& namespaces, const std::string& className, const std::string& name);
		static Optional<ParserVariable> GetVariable(const Language::Namespace& namespaces, const std::string& className, const std::string& name);
	
	private:
		static void PopVars(size_t count);
	
	private:
		inline static std::vector<ParserVariable> s_Variables = {};
		inline static std::vector<size_t> s_Scopes = { 0 };
	};

}
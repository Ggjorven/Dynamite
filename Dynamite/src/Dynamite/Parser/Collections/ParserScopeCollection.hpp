#pragma once

#include "Core/Core.hpp"

#include "Language/Types/Type.hpp"

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
		std::string Name;
		Language::Type VariableType;
	};

	/////////////////////////////////////////////////////////////////
	// ParserScopeCollection
	/////////////////////////////////////////////////////////////////
	class ParserScopeCollection
	{
	public:
		void Reset();

		void BeginScope();
		void EndScope();

		void PushVar(const std::string& name, const Language::Type& type);

		Optional<Language::Type> GetVariableType(const std::string& name);
	
	private:
		void PopVars(size_t count);
	
	private:
		std::vector<ParserVariable> m_Variables = {};
		std::vector<size_t> m_Scopes = { 0 };
	};

}
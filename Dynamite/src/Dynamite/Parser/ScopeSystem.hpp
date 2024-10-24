#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// ScopeSystem
	/////////////////////////////////////////////////////////////////
	class ScopeSystem
	{
	public:
		struct Variable
		{
		public:
			std::string Name;
			Type VariableType;
		};
	public:
		void Reset();

		void BeginScope();
		void EndScope();

		void PushVar(const std::string& name, const Type& type);
		void PopVars(size_t count);

		Optional<Type> GetVariableType(const std::string& name) const;

	private:
		std::vector<Variable> m_Variables = {};
		std::vector<size_t> m_Scopes = { 0 };
	};

}
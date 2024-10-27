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
	class ScopeSystem // Note: All systems are static for debug/printing purposes
	{
	public:
		struct Variable
		{
		public:
			std::string Name;
			Type VariableType;
		};
	public:
		static void Reset();

		static void BeginScope();
		static void EndScope();

		static void PushVar(const std::string& name, const Type& type);
		static void PopVars(size_t count);

		static Optional<Type> GetVariableType(const std::string& name);

	private:
		inline static std::vector<Variable> s_Variables = {};
		inline static std::vector<size_t> s_Scopes = { 0 };
	};

}
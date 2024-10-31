#include "dypch.h"
#include "ScopeSystem.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// ScopeSystem
	/////////////////////////////////////////////////////////////////
	void ScopeSystem::Reset()
	{
		s_Variables.clear();
		s_Scopes = { 0 };
	}

	void ScopeSystem::BeginScope()
	{
		s_Scopes.push_back(s_Variables.size());
	}

	void ScopeSystem::EndScope()
	{
		PopVars(s_Variables.size() - s_Scopes.back());
		s_Scopes.pop_back();
	}

	void ScopeSystem::PushVar(const std::string& name, const Type& type)
	{
		s_Variables.emplace_back(name, type);
	}

	void ScopeSystem::PopVars(size_t count)
	{
		if (count <= s_Variables.size())
			s_Variables.resize(s_Variables.size() - count);
		else
			s_Variables.clear(); 
	}

	Optional<Type> ScopeSystem::GetVariableType(const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Variables), [&](const Variable& var) { return var.Name == name; });
		if (it == s_Variables.cend()) return {};

		return (*it).VariableType;
	}

}
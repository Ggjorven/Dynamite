#include "dypch.h"
#include "ScopeSystem.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// ScopeSystem
	/////////////////////////////////////////////////////////////////
	void ScopeSystem::Reset()
	{
		m_Variables.clear();
		m_Scopes = { 0 };
	}

	void ScopeSystem::BeginScope()
	{
		m_Scopes.push_back(m_Variables.size());
	}

	void ScopeSystem::EndScope()
	{
		PopVars(m_Variables.size() - m_Scopes.back());
		m_Scopes.pop_back();
	}

	void ScopeSystem::PushVar(const std::string& name, const Type& type)
	{
		m_Variables.emplace_back(name, type);
	}

	void ScopeSystem::PopVars(size_t count)
	{
		if (count <= m_Variables.size())
			m_Variables.resize(m_Variables.size() - count);
		else
			m_Variables.clear(); 
	}

	Optional<Type> ScopeSystem::GetVariableType(const std::string& name) const
	{
		const auto it = std::ranges::find_if(std::as_const(m_Variables), [&](const Variable& var)
		{
			return var.Name == name;
		});

		if (it == m_Variables.cend())
		{
			//Compiler::Error(GetLineNumber(), "Undeclared identifier: {0}", name);
			return {};
		}

		return (*it).VariableType;
	}

}
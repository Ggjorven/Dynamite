#include "dypch.h"
#include "ParserScopeCollection.hpp"

#include "Core/Logging.hpp"

#include "Utils/Utils.hpp"
#include "Utils/Checks.hpp"

#include "Compiler/Compiler.hpp"

namespace Dynamite
{

	using namespace Language;

	/////////////////////////////////////////////////////////////////
	// ParserScopeCollection
	/////////////////////////////////////////////////////////////////
	void ParserScopeCollection::Reset()
	{
		m_Variables.clear();
		m_Scopes = { 0 };
	}

	void ParserScopeCollection::BeginScope()
	{
		m_Scopes.push_back(m_Variables.size());
	}

	void ParserScopeCollection::EndScope()
	{
		PopVars(m_Variables.size() - m_Scopes.back());
		m_Scopes.pop_back();
	}

	void ParserScopeCollection::PushVar(const std::string& name, const Type& type)
	{
		m_Variables.emplace_back(name, type);
	}

	Optional<Type> ParserScopeCollection::GetVariableType(const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Variables), [&](const ParserVariable& var) { return var.Name == name; });
		if (it == m_Variables.cend()) return {};

		return (*it).VariableType;
	}

	void ParserScopeCollection::PopVars(size_t count)
	{
		if (count <= m_Variables.size())
			m_Variables.resize(m_Variables.size() - count);
		else
			m_Variables.clear();
	}

}
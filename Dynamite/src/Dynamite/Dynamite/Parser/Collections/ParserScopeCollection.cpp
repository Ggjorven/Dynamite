#include "dypch.h"
#include "ParserScopeCollection.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

namespace Dynamite
{

	using namespace Language;

	/////////////////////////////////////////////////////////////////
	// ParserScopeCollection
	/////////////////////////////////////////////////////////////////
	void ParserScopeCollection::Reset()
	{
		s_Variables.clear();
		s_Scopes = { 0 };
	}

	void ParserScopeCollection::BeginScope()
	{
		s_Scopes.push_back(s_Variables.size());
	}

	void ParserScopeCollection::EndScope()
	{
		// Pop variables
		{
			size_t popCount = s_Variables.size() - s_Scopes.back();
			if (popCount <= s_Variables.size())
				s_Variables.resize(s_Variables.size() - popCount);
			else
				s_Variables.clear();
		}

		s_Scopes.pop_back();
	}

	void ParserScopeCollection::PushVar(const ParserVariable& variable)
	{
		s_Variables.push_back(variable);
	}

	bool ParserScopeCollection::Exists(const Language::Namespace& namespaces, const std::string& className, const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Variables), [&](const ParserVariable& var) 
		{ 
			return var.Namespaces == namespaces && var.ClassName == className && var.Name == name; 
		});

		return (it != s_Variables.cend());
	}

	Optional<ParserVariable> ParserScopeCollection::GetVariable(const Language::Namespace& namespaces, const std::string& className, const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Variables), [&](const ParserVariable& var)
		{
			return var.Namespaces == namespaces && var.ClassName == className && var.Name == name;
		});
		if (it == s_Variables.cend()) return {};

		return (*it);
	}

	void ParserScopeCollection::PopVars(size_t count)
	{
		if (count <= s_Variables.size())
			s_Variables.resize(s_Variables.size() - count);
		else
			s_Variables.clear();
	}

}
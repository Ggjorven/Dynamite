#include "dypch.h"
#include "IRScopeCollection.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// IRScopeCollection
	/////////////////////////////////////////////////////////////////
	void IRScopeCollection::Reset()
	{
		s_Variables.clear();
		s_Scopes = { 0 };
	}

	void IRScopeCollection::BeginScope()
	{
		s_Scopes.push_back(s_Variables.size());
	}

	void IRScopeCollection::EndScope()
	{
		PopVars(s_Variables.size() - s_Scopes.back());
		s_Scopes.pop_back();
	}

	void IRScopeCollection::PushVar(const std::string& name, const Type& type, llvm::Value* value)
	{
		s_Variables.emplace_back(name, type, value);
	}

	IRScopeCollection::Variable& IRScopeCollection::GetVariable(const std::string& name)
	{
		auto it = std::ranges::find_if(s_Variables, [&](const Variable& var) { return var.Name == name; });

		return (*it);
	}

	llvm::Value* IRScopeCollection::GetLLVMValue(const std::string& name)
	{
		return GetVariable(name).LLVMValue;
	}

	void IRScopeCollection::PopVars(size_t count)
	{
		if (count <= s_Variables.size())
			s_Variables.resize(s_Variables.size() - count);
		else
			s_Variables.clear();
	}

}
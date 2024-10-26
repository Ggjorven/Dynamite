#include "dypch.h"
#include "FunctionSystem.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Types/TypeSystem.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	void FunctionSystem::Reset()
	{
	}

	void FunctionSystem::Add(const std::string& name, Type returnType, const std::vector<Type>& parameters)
	{
		m_Functions.emplace_back(name, returnType, parameters);
	}

	Optional<Type> FunctionSystem::GetReturnType(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == m_Functions.cend())
			return {};

		return Optional<Type>((*it).ReturnType);
	}

	Optional<Type> FunctionSystem::GetArgumentType(const std::string& functionName, size_t index)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == m_Functions.cend())
			return {};
		else if (index >= (*it).Parameters.size())
			return {};

		return Optional<Type>((*it).Parameters[index]);
	}

}
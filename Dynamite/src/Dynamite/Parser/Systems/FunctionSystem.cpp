#include "dypch.h"
#include "FunctionSystem.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	void FunctionSystem::Reset()
	{
		s_Functions.clear();
	}

	void FunctionSystem::Add(const std::string& name, Type returnType, const std::vector<std::pair<Type, bool>>& parameters)
	{
		s_Functions.emplace_back(name, returnType, parameters);
	}

	Optional<Type> FunctionSystem::GetReturnType(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend())
			return {};

		return Optional<Type>((*it).ReturnType);
	}

	Optional<Type> FunctionSystem::GetArgumentType(const std::string& functionName, size_t index)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend())
			return {};
		else if (index >= (*it).Parameters.size())
			return {};

		return Optional<Type>((*it).Parameters[index].first);
	}

	size_t FunctionSystem::GetArgCount(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend())
			return 0;

		return (*it).Parameters.size();
	}

	size_t FunctionSystem::GetRequiredArgCount(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend() || it->Parameters.empty())
			return 0;

		auto& parameters = it->Parameters;
		size_t requiredCount = parameters.size();

		for (size_t i = parameters.size(); i-- > 0;)
		{
			auto& [type, required] = parameters[i];
			if (required)
			{
				requiredCount = i + 1;
				break;
			}
		}

		return requiredCount;
	}


}
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
		s_Functions.emplace_back(name, Function::Overload(returnType, parameters));
	}

	Optional<Type> FunctionSystem::GetReturnType(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend())
			return {};

		return Optional<Type>((*it).OverloadObj.ReturnType);
	}

	Optional<Type> FunctionSystem::GetArgumentType(const std::string& functionName, size_t index)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend())
			return {};
		else if (index >= (*it).OverloadObj.Parameters.size())
			return {};

		return Optional<Type>((*it).OverloadObj.Parameters[index].first);
	}

	size_t FunctionSystem::GetArgCount(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend())
			return 0;

		return (*it).OverloadObj.Parameters.size();
	}

	size_t FunctionSystem::GetRequiredArgCount(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == functionName;
		});

		if (it == s_Functions.cend() || it->OverloadObj.Parameters.empty())
			return 0;

		auto& parameters = it->OverloadObj.Parameters;
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

	bool FunctionSystem::Exists(const std::string& name, const Type& returnType, const std::vector<Type>& parameters)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func)
		{
			return func.Name == name;
		});

		if (it == s_Functions.cend())
			return false;

		// Checks
		if ((*it).OverloadObj.ReturnType != returnType)
			return false;

		if ((*it).OverloadObj.Parameters.size() != parameters.size())
			return false;

		for (size_t i = 0; i < parameters.size(); i++)
		{
			if ((*it).OverloadObj.Parameters[i].first != parameters[i])
				return false;
		}

		return true;
	}


}
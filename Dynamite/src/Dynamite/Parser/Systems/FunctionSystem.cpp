#include "dypch.h"
#include "FunctionSystem.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	bool FunctionSystem::Function::Overload::operator == (const FunctionSystem::Function::Overload& other)
	{
		if (this->ReturnType != other.ReturnType)
			return false;

		if (this->Parameters.size() != other.Parameters.size())
			return false;

		for (size_t i = 0; i < this->Parameters.size(); i++)
		{
			if (this->Parameters[i].first != other.Parameters[i].first)
				return false;
		}

		return true;
	}

	bool FunctionSystem::Function::Overload::operator != (const FunctionSystem::Function::Overload& other)
	{
		return !(*this == other);
	}

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	void FunctionSystem::Reset()
	{
		s_Functions.clear();
	}

	void FunctionSystem::Add(const std::string& name, Type returnType, const std::vector<std::pair<Type, bool>>& parameters)
	{
		auto it = std::ranges::find_if(s_Functions, [&](const Function& func) { return func.Name == name; });
		if (it == s_Functions.cend()) 
		{
			s_Functions.emplace_back(name, std::vector({ Function::Overload(returnType, parameters) }));
		}
		else
		{
			Function& func = *it;
			func.Overloads.emplace_back(returnType, parameters);
		}
	}

	std::vector<Type> FunctionSystem::GetReturnTypes(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func) { return func.Name == functionName; });
		if (it == s_Functions.cend()) return { };
		const Function& func = *it;

		std::vector<Type> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
			result.push_back(overload.ReturnType);

		return result;
	}

	std::vector<Type> FunctionSystem::GetArgumentTypes(const std::string& functionName, size_t index)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func) { return func.Name == functionName; });
		if (it == s_Functions.cend()) return { };
		const Function& func = *it;
		
		std::vector<Type> result;
		result.reserve(func.Overloads.size());
		
		for (const auto& overload : func.Overloads)
		{
			if (index < overload.Parameters.size())
				result.push_back(overload.Parameters[index].first);
		}

		return result;
	}

	std::vector<size_t> FunctionSystem::GetArgCounts(const std::string & functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func) { return func.Name == functionName; });
		if (it == s_Functions.cend()) return { };
		const Function& func = *it;

		std::vector<size_t> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
			result.push_back(overload.Parameters.size());

		return result;
	}

	std::vector<size_t> FunctionSystem::GetRequiredArgCounts(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func) { return func.Name == functionName; });
		if (it == s_Functions.cend()) return { };
		const Function& func = *it;

		std::vector<size_t> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
		{
			bool broke = false;

			// Reverse iterate through the parameters
			for (size_t i = (overload.Parameters.size() - 1); i > 0; i--)
			{
				auto& [type, required] = overload.Parameters[i];
				if (required)
				{
					result.push_back(i + 1);

					broke = true;
					break;
				}
			}

			if (!broke)
				result.push_back((overload.Parameters.size()));
		}

		return result;
	}

	bool FunctionSystem::Exists(const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func) { return func.Name == name; });

		return (it != s_Functions.cend());
	}

	bool FunctionSystem::Exists(const std::string& name, const Type& returnType, const std::vector<Type>& parameters)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const Function& func) { return func.Name == name; });
		if (it == s_Functions.cend()) return false;
		const Function& func = *it;

		std::vector<size_t> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
		{
			if ((returnType == overload.ReturnType) && (parameters.size() == overload.Parameters.size()))
			{
				bool failed = false;
				for (size_t i = 0; i < parameters.size(); i++)
				{
					if (parameters[i] != overload.Parameters[i].first)
					{
						failed = true;
						break;
					}

				}

				return !failed;
			}
		}

		return false;
	}

	FunctionSystem::Function* FunctionSystem::GetFunction(const std::string& name)
	{
		auto it = std::ranges::find_if(s_Functions, [&](const Function& func) { return func.Name == name; });
		if (it == s_Functions.cend()) return nullptr;

		return &(*it);
	}

	FunctionSystem::Function::Overload* FunctionSystem::GetOverload(const std::string& name, size_t overloadIndex)
	{
		auto it = std::ranges::find_if(s_Functions, [&](const Function& func) { return func.Name == name; });
		if (it == s_Functions.cend()) return nullptr;

		return &((*it).Overloads[overloadIndex]);
	}

}
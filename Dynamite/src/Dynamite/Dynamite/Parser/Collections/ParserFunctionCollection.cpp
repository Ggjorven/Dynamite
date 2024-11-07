#include "dypch.h"
#include "ParserFunctionCollection.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

namespace Dynamite
{

	using namespace Language;

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	bool ParserFunction::Overload::operator == (const ParserFunction::Overload& other)
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

	bool ParserFunction::Overload::operator != (const ParserFunction::Overload& other)
	{
		return !(*this == other);
	}

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	void ParserFunctionCollection::Reset()
	{
		m_Functions.clear();
	}

	void ParserFunctionCollection::Add(const std::string& name, Type returnType, const std::vector<std::pair<Type, bool>>& parameters, bool hasCStyleVardiadicArguments)
	{
		auto it = std::ranges::find_if(m_Functions, [&](const ParserFunction& func) { return func.Name == name; });
		if (it == m_Functions.cend())
		{
			m_Functions.emplace_back(name, std::vector({ ParserFunction::Overload(returnType, parameters, hasCStyleVardiadicArguments) }));
		}
		else
		{
			ParserFunction& func = *it;
			func.Overloads.emplace_back(returnType, parameters, hasCStyleVardiadicArguments);
		}
	}

	std::vector<Type> ParserFunctionCollection::GetReturnTypes(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const ParserFunction& func) { return func.Name == functionName; });
		if (it == m_Functions.cend()) return { };
		const ParserFunction& func = *it;

		std::vector<Type> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
			result.push_back(overload.ReturnType);

		return result;
	}

	std::vector<Type> ParserFunctionCollection::GetArgumentTypes(const std::string& functionName, size_t index)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const ParserFunction& func) { return func.Name == functionName; });
		if (it == m_Functions.cend()) return { };
		const ParserFunction& func = *it;
		
		std::vector<Type> result;
		result.reserve(func.Overloads.size());
		
		for (const auto& overload : func.Overloads)
		{
			if (index < overload.Parameters.size())
				result.push_back(overload.Parameters[index].first);
		}

		return result;
	}

	std::vector<size_t> ParserFunctionCollection::GetArgCounts(const std::string & functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const ParserFunction& func) { return func.Name == functionName; });
		if (it == m_Functions.cend()) return { };
		const ParserFunction& func = *it;

		std::vector<size_t> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
			result.push_back(overload.Parameters.size());

		return result;
	}

	std::vector<size_t> ParserFunctionCollection::GetRequiredArgCounts(const std::string& functionName)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const ParserFunction& func) { return func.Name == functionName; });
		if (it == m_Functions.cend()) return { };
		const ParserFunction& func = *it;

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

	bool ParserFunctionCollection::Exists(const std::string& name)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const ParserFunction& func) { return func.Name == name; });

		return (it != m_Functions.cend());
	}

	bool ParserFunctionCollection::Exists(const std::string& name, const Type& returnType, const std::vector<Type>& parameters, bool hasVardiadicArguments)
	{
		const auto it = std::ranges::find_if(std::as_const(m_Functions), [&](const ParserFunction& func) { return func.Name == name; });
		if (it == m_Functions.cend()) return false;
		const ParserFunction& func = *it;

		std::vector<size_t> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
		{
			if ((returnType == overload.ReturnType) && (parameters.size() == overload.Parameters.size()) && (hasVardiadicArguments == overload.CStyleVardiadicArguments))
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

	ParserFunction* ParserFunctionCollection::GetFunction(const std::string& name)
	{
		auto it = std::ranges::find_if(m_Functions, [&](const ParserFunction& func) { return func.Name == name; });
		if (it == m_Functions.cend()) return nullptr;

		return &(*it);
	}

	ParserFunction::Overload* ParserFunctionCollection::GetOverload(const std::string& name, size_t overloadIndex)
	{
		auto it = std::ranges::find_if(m_Functions, [&](const ParserFunction& func) { return func.Name == name; });
		if (it == m_Functions.cend()) return nullptr;

		return &((*it).Overloads[overloadIndex]);
	}

}
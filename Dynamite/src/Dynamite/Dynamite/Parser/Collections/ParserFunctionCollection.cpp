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
	std::vector<Language::Type> ParserFunction::Overload::GetArgumentTypes()
	{
		std::vector<Type> result;
		result.reserve(Parameters.size());

		for (const auto& [type, required] : Parameters)
			result.push_back(type);

		return result;
	}

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

	std::vector<Language::Type> ParserFunction::GetReturnTypes() const
	{
		std::vector<Type> result;
		result.reserve(Overloads.size());

		for (const auto& overload : Overloads)
			result.push_back(overload.ReturnType);

		return result;
	}

	std::vector<size_t> ParserFunction::GetArgCounts() const
	{
		std::vector<size_t> result;
		result.reserve(Overloads.size());

		for (const auto& overload : Overloads)
			result.push_back(overload.Parameters.size());

		return result;
	}

	std::vector<size_t> ParserFunction::GetRequiredArgCounts() const
	{
		std::vector<size_t> result;
		result.reserve(Overloads.size());

		for (const auto& overload : Overloads)
		{
			bool broke = false;

			// Handle the case when there are no parameters
			if (overload.Parameters.empty())
			{
				result.push_back(0);
				continue;
			}

			// Reverse iterate through the parameters
			for (size_t i = overload.Parameters.size(); i > 0; --i)
			{
				auto& [type, required] = overload.Parameters[i - 1];
				if (required)
				{
					result.push_back(i);
					broke = true;
					break;
				}
			}

			if (!broke)
				result.push_back(overload.Parameters.size());
		}

		return result;
	}

	std::string ParserFunction::GetFunctionName(size_t overloadIndex) const
	{
		if (overloadIndex >= Overloads.size())
		{
			DY_LOG_ERROR("[Internal Compiler Error] - Accessing a non existing overloads name.");
			return {};
		}

		return ParserFunction::ConstructName(Namespaces, Name, overloadIndex);
	}

	std::string ParserFunction::ConstructName(const Language::Namespace& namespaces, const std::string& name, size_t overloadIndex)
	{
		std::string funcName = "Dy_";

		for (const std::string& nameSpace : namespaces.GetAllLevels())
			funcName += nameSpace + '_';

		funcName += name + '_';
		funcName += std::to_string(overloadIndex);

		return funcName;
	}

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	void ParserFunctionCollection::Reset()
	{
		s_Functions.clear();
	}

	void ParserFunctionCollection::Add(const ParserFunction& func)
	{
		s_Functions.push_back(func);
	}

	void ParserFunctionCollection::Add(const Language::Namespace& namespaces, const std::string& name, const ParserFunction::Overload& overload)
	{
		auto it = std::ranges::find_if(s_Functions, [&](const ParserFunction& func) 
		{ 
			return func.Namespaces == namespaces && func.Name == name;
		});

		if (it == s_Functions.end())
		{
			s_Functions.emplace_back(namespaces, name, std::vector({ overload }));
		}
		else
		{
			ParserFunction& func = *it;
			func.Overloads.push_back(overload);
		}
	}

	bool ParserFunctionCollection::Exists(const Language::Namespace& namespaces, const std::string& name, const Language::Type& returnType, const std::vector<Language::Type>& parameters, bool hasCStyleVardiadicArguments)
	{
		const auto it = std::ranges::find_if(std::as_const(s_Functions), [&](const ParserFunction& func)
		{
			return func.Namespaces == namespaces && func.Name == name;
		});
		if (it == s_Functions.cend()) return false;
		const ParserFunction& func = *it;

		std::vector<size_t> result;
		result.reserve(func.Overloads.size());

		for (const auto& overload : func.Overloads)
		{
			if ((returnType == overload.ReturnType) && (parameters.size() == overload.Parameters.size()) && (hasCStyleVardiadicArguments == overload.CStyleVardiadicArguments))
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

	Optional<ParserFunction> ParserFunctionCollection::GetFunction(const Language::Namespace& namespaces, const std::string& name)
	{
		auto it = std::ranges::find_if(s_Functions, [&](const ParserFunction& func)
		{
			return func.Namespaces == namespaces && func.Name == name;
		});
		if (it == s_Functions.cend()) return {};

		return (*it);
	}

}
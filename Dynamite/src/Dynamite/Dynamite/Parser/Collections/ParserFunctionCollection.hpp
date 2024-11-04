#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	struct ParserFunction
	{
	public:
		struct Overload
		{
		public:
			Language::Type ReturnType;
			std::vector<std::pair<Language::Type, bool>> Parameters; // Type & required bool
			bool VardiadicArguments;

		public:
			bool operator == (const Overload& other);
			bool operator != (const Overload& other);
		};
	public:
		std::string Name;
		std::vector<Overload> Overloads;
	};

	/////////////////////////////////////////////////////////////////
	// ParserFunctionCollection
	/////////////////////////////////////////////////////////////////
	class ParserFunctionCollection // Note: All systems are static for debug/printing purposes
	{
	public:
		void Reset();

		void Add(const std::string& name, Language::Type returnType, const std::vector<std::pair<Language::Type, bool>>& parameters = { }, bool hasVardiadicArguments = false);

		std::vector<Language::Type> GetReturnTypes(const std::string& functionName);
		std::vector<Language::Type> GetArgumentTypes(const std::string& functionName, size_t index);

		std::vector<size_t> GetArgCounts(const std::string& functionName);
		std::vector<size_t> GetRequiredArgCounts(const std::string& functionName);

		bool Exists(const std::string& name);
		bool Exists(const std::string& name, const Language::Type& returnType, const std::vector<Language::Type>& parameters = { }, bool hasVardiadicArguments = false);

		ParserFunction* GetFunction(const std::string& name);
		ParserFunction::Overload* GetOverload(const std::string& name, size_t overloadIndex = 0);

	private:
		std::vector<ParserFunction> m_Functions = {};
	};

}
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/Utils/Namespace.hpp"

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
			std::vector<std::pair<Language::Type, bool>> Parameters; // Type & required bool (has default value)
			
			// Note: Exists for compatibility resons
			bool CStyleVardiadicArguments;

		public:
			// Methods
			std::vector<Language::Type> GetArgumentTypes();

			// Operators
			bool operator == (const Overload& other);
			bool operator != (const Overload& other);
		};
	public:
		Language::Namespace Namespaces;

		std::string Name;
		std::vector<Overload> Overloads;

	public:
		// Methods for overloads
		std::vector<Language::Type> GetReturnTypes() const;

		std::vector<size_t> GetArgCounts() const;
		std::vector<size_t> GetRequiredArgCounts() const;

		// Utility
		std::string GetFunctionName(size_t overloadIndex = 0) const;

	public:
		static std::string ConstructName(const Language::Namespace& namespaces, const std::string& name, size_t overloadIndex = 0);
	};

	/////////////////////////////////////////////////////////////////
	// ParserFunctionCollection
	/////////////////////////////////////////////////////////////////
	class ParserFunctionCollection
	{
	public:
		static void Reset();

		static void Add(const ParserFunction& func);
		static void Add(const Language::Namespace& namespaces, const std::string& name, const ParserFunction::Overload& overload);

		static bool Exists(const Language::Namespace& namespaces, const std::string& name, const Language::Type& returnType, const std::vector<Language::Type>& parameters = { }, bool hasCStyleVardiadicArguments = false);

		Optional<ParserFunction> GetFunction(const Language::Namespace& namespaces, const std::string& name);

	private:
		inline static std::vector<ParserFunction> s_Functions = {};
	};

}
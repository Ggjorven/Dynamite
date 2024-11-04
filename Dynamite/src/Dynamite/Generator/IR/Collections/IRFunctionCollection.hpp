#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/Collections/IRScopeCollection.hpp"

#include <cstdint>
#include <utility>
#include <filesystem>
#include <unordered_map>

#include <llvm/IR/IRBuilder.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	struct GeneratorFunction
	{
	public:
		llvm::FunctionCallee Callee;
		std::vector<GeneratorVariable> Arguments;
		bool HasVardiadicArguments;
	};

	/////////////////////////////////////////////////////////////////
	// IRFunctionCollection
	/////////////////////////////////////////////////////////////////
	class IRFunctionCollection
	{
	public:
		static void Reset();

		static void AddFunction(const std::string& name, const GeneratorFunction& func);

		static GeneratorFunction& GetFunction(const std::string& name, size_t overloadIndex = 0);
	
	public:
		// Note: Is in the same order as the Parser's FunctionSystem
		// Is name, then the overloads vector.
		inline static std::unordered_map<std::string, std::vector<GeneratorFunction>> s_Functions = { };
	};

}
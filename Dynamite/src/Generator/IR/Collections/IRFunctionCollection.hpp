#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Program.hpp"

#include "Generator/Types/GenTypes.hpp"

#include "Generator/IR/Collections/IRScopeCollection.hpp"

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
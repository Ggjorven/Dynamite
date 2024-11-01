#pragma once

#include "Core/Core.hpp"

#include "Language/Types/Type.hpp"

#include "Generator/Types/GenTypes.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	struct GeneratorVariable
	{
	public:
		std::string Name;
		Type VariableType;
		GeneratorValue Value;
	};

	/////////////////////////////////////////////////////////////////
	// IRScopeCollection
	/////////////////////////////////////////////////////////////////
	class IRScopeCollection
	{
	public:
		static void Reset();

		static void BeginScope();
		static void EndScope();

		static void PushVar(const std::string& name, const Type& type, const GeneratorValue& value);

		// Note: This is a reference, so we trust the user to put in a valid name
		static GeneratorVariable& GetVariable(const std::string& name);

	private:
		static void PopVars(size_t count);

	private:
		inline static std::vector<GeneratorVariable> s_Variables = {};
		inline static std::vector<size_t> s_Scopes = { 0 };
	};

}
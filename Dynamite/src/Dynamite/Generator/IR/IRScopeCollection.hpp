#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

#include <llvm/IR/IRBuilder.h>

namespace Dynamite
{

	class IRScopeCollection
	{
	public:
		struct Variable
		{
		public:
			std::string Name;
			Type VariableType;

			llvm::Value* LLVMValue;
		};
	public:
		static void Reset();

		static void BeginScope();
		static void EndScope();

		static void PushVar(const std::string& name, const Type& type, llvm::Value* value);

		// Note: This is a reference, so we trust the user to put in a valid name
		static Variable& GetVariable(const std::string& name);
		static llvm::Value* GetLLVMValue(const std::string& name);

	private:
		static void PopVars(size_t count);

	private:
		inline static std::vector<Variable> s_Variables = {};
		inline static std::vector<size_t> s_Scopes = { 0 };
	};

}
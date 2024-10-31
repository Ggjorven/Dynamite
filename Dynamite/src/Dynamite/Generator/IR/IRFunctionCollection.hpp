#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <utility>
#include <filesystem>
#include <unordered_map>

#include <llvm/IR/IRBuilder.h>

namespace Dynamite
{

	class IRFunctionCollection
	{
	public:
		struct Function
		{
		public:
			llvm::FunctionCallee Callee;
			std::vector<std::pair<llvm::Value*, Type>> Arguments;
		};
	public:
		static void Reset();

		static void AddFunction(const std::string& name, const Function& func);

		static Function& GetFunction(const std::string& name, size_t overloadIndex = 0);
		static llvm::FunctionCallee& GetLLVMFunction(const std::string& name, size_t overloadIndex = 0);
		static llvm::Value* GetLLVMArg(const std::string& funcName, size_t argIndex, size_t overloadIndex = 0);
		static Type& GetLLVMArgType(const std::string& funcName, size_t argIndex, size_t overloadIndex = 0);
	
	public:
		// Note: Is in the same order as the Parser's FunctionSystem
		// Is name, then the overloads vector.
		inline static std::unordered_map<std::string, std::vector<Function>> s_Functions = { };
	};

}
#pragma once

#include "Dynamite/Generator/Generator.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace Dynamite
{

	class LLVMIRGenerator
	{
	public:
		LLVMIRGenerator() = default;
		~LLVMIRGenerator() = default;

		void Generate(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod);
	};

}
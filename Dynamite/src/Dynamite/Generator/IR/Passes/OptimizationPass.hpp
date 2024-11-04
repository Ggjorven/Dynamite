#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Optimization pass
	/////////////////////////////////////////////////////////////////
	class OptimizationPass
	{
	public:
		// Constructors
		OptimizationPass();

		// Public methods
		void Run(llvm::Module& mod);
	};

}
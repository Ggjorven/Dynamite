#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <string>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Intermediate pass
	/////////////////////////////////////////////////////////////////
	class IRPass
	{
	public:
		// Constructors
		IRPass();

		// Public methods
		void Run(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod);
	};

}
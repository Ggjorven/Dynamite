#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Program.hpp"

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
		IRPass(std::string& generatedIR);

		// Public methods
		void Generate(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod);

	private:
		std::string& m_GeneratedIR;
	};

}
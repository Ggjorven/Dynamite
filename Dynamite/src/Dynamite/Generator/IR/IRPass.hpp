#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace Dynamite
{

	class IRPass
	{
	public:
		static void Generate(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod);
	};

}
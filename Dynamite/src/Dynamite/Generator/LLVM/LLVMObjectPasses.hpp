#pragma once

#include <llvm/IR/Module.h>

namespace Dynamite
{

	class LLVMObjectPasses
	{
	public:
		LLVMObjectPasses() = default;
		~LLVMObjectPasses() = default;

		void Run(llvm::Module& mod, const std::filesystem::path& objectPath);
	};

}
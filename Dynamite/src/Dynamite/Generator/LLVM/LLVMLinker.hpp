#pragma once

#include <llvm/IR/Module.h>

#include <vector>
#include <filesystem>

namespace Dynamite
{

	class LLVMLinker
	{
	public:
		static void Link(const std::filesystem::path& outputDir, const std::vector<std::filesystem::path>& objectFiles);
	};

}
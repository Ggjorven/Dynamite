#include "dypch.h"
#include "Linker.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Generator/LLVM/LLVMLinker.hpp"

namespace Dynamite
{

	void Linker::Link(const std::filesystem::path& outputDir, const std::vector<std::filesystem::path>& objectFiles)
	{
		LLVMLinker::Link(outputDir, objectFiles);
	}

}
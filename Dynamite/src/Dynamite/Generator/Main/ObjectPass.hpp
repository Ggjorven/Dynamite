#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <filesystem>

#include <llvm/IR/Module.h>

namespace Dynamite
{

	class ObjectPass
	{
	public:
		static void Run(llvm::Module& mod, const std::filesystem::path& objectPath);
	};

}
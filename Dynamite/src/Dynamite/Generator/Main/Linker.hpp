#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

namespace Dynamite
{

	class Linker
	{
	public:
		static void Link(const std::filesystem::path& outputDir, const std::vector<std::filesystem::path>& objectFiles);
	};

}
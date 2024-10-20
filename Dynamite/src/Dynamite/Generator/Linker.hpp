#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Nodes.hpp"
#include "Dynamite/Parsing/Variables.hpp"

#include <Pulse/Core/Unique.hpp>

#include <filesystem>

namespace Dynamite
{

	class Linker
	{
	public:
		static void Link(const std::filesystem::path& outputDir, const std::vector<std::filesystem::path>& objectFiles);
	};

}
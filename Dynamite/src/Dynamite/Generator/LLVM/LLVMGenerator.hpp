#pragma once

#include "Dynamite/Generator/Generator.hpp"

namespace Dynamite
{

	class LLVMGenerator : public Generator
	{
	public:
		LLVMGenerator() = default;
		~LLVMGenerator() = default;

		void Generate(Node::Program& program, const std::filesystem::path& outputPath) override;
	};

}
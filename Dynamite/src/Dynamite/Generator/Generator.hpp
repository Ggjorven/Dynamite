#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <string>
#include <filesystem>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Generator
	/////////////////////////////////////////////////////////////////
	class Generator
	{
	public:
		// Constructors
		Generator(Language::Node::Program& program, std::string& generatedIR);
		~Generator() = default;

		// Public methods
		void Generate(const std::filesystem::path& outputPath);

	private:
		Language::Node::Program& m_Program;
		std::string& m_GeneratedIR;
	};

}
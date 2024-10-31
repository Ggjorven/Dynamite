#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

namespace Dynamite
{

	class Generator
	{
	public:
		// Constructors
		Generator(Node::Program& program);
		~Generator() = default;

		// Public methods
		void Generate(const std::filesystem::path& outputPath);

	private:

	private:
		Node::Program& m_Program;
	};

}
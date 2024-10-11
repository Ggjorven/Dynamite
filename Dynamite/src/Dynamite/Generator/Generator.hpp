#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Nodes.hpp"
#include "Dynamite/Parsing/Variables.hpp"

#include <Pulse/Core/Unique.hpp>

#include <filesystem>

namespace Dynamite
{

	class Generator
	{
	public:
		enum class Type : uint8_t { ASM, LLVM /*Not implemented.*/ };
	public:
		Generator() = default;
		virtual ~Generator() = default;
	
		// Note: The extension of the outputPath is irrelevant
		virtual void Generate(Node::Program& program, const std::filesystem::path& outputPath) = 0;

		static Pulse::Unique<Generator> Create(Type type = Type::ASM);
	};

}
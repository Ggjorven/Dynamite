#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

namespace Dynamite
{

	enum class CompilerFlag : uint8_t	
	{ 
		None = 0, 
		
		Help = 1 << 0, 
		Verbose = 1 << 1, 
		Debug = 1 << 2 
	};

	struct CompilerOptions
	{
	public:
		constexpr static const char* OutputDirectory = "bin";
		constexpr static const char* IntermediateDirectory = "int";
	public:
		int Argc;
		char** Argv;

	public:
		std::filesystem::path WorkingDir;

		std::vector<std::filesystem::path> Files = { };
		std::vector<std::filesystem::path> IncludeDirs = { };

		std::filesystem::path OutputDir;

		CompilerFlag Flags = CompilerFlag::None;

	public:
		CompilerOptions() = default;
		CompilerOptions(int argc, char** argv);
		~CompilerOptions() = default;

		// Returns true if there is a flag with type value
		bool Contains(CompilerFlag type) const;

	private:
		void PrintHelpMenu();

		void ParseArgs(int argc, char** argv);
	};

}
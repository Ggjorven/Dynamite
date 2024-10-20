#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace Dynamite
{

	enum class CompilerFlag : uint8_t
	{
		None = 0, Verbose = 1 << 0,
	};

	struct CompilerOptions
	{
	public:
		constexpr static const char* IntermediatePath = "int";
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
		CompilerOptions(int argc, char** argv);

		// Returns true if there is a flag with type value
		const bool Contains(CompilerFlag type) const;

	public:
		void ParseArgs(int argc, char** argv);
	};

}
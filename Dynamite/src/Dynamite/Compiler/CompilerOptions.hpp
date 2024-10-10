#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace Dynamite
{

	struct CompilerFlag
	{
	public:
		enum class Type : uint8_t { None = 0, File, IncludeDir, OutputDir, Verbose };
	public:
		Type Flag;
		const std::optional<std::string> Value;

	public:
		CompilerFlag(const std::optional<std::string>& value);
		CompilerFlag(Type flag = Type::None, const std::optional<std::string>& value = {});

		static std::string Format(const CompilerFlag& flag);
	};

	struct CompilerOptions
	{
	public:
		std::vector<CompilerFlag> Flags = { };
		std::filesystem::path WorkingDir;

	public:
		CompilerOptions(int argc, char** argv);

		// Returns the values of all flags with current type
		std::vector<std::string> Get(CompilerFlag::Type type) const;

		// Returns true if there is a flag with type value
		const bool Contains(CompilerFlag::Type type) const;

	public:
		void ParseArgs(int argc, char** argv);
	};

}
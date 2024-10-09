#include "dypch.h"
#include "CompilerOptions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

	namespace
	{
		std::optional<CompilerFlag> GetFlag(const std::string& str)
		{
			if (str.empty() || str.size() < 2)
				return {};

			if (str.substr(0, 2) != "--")
				return CompilerFlag(CompilerFlag::Type::File, str);

			if (str.substr(2, 2) == "I=")
				return CompilerFlag(CompilerFlag::Type::IncludeDir, str.substr(4, str.size() - 4));

			return {};
		}
	}

	CompilerFlag::CompilerFlag(const std::optional<std::string>& value)
		: Flag(Type::File), Value(value)
	{
	}

	CompilerFlag::CompilerFlag(Type flag, const std::optional<std::string>& value)
		: Flag(flag), Value(value)
	{
	}

	std::string CompilerFlag::Format(const CompilerFlag& flag)
	{
		if (flag.Value.has_value())
			return Pulse::Text::Format("CompilerFlag::Type::{0} - {1}", Pulse::Enum::Name(flag.Flag), flag.Value.value());
		else
			return Pulse::Text::Format("CompilerFlag::Type::{0}", Pulse::Enum::Name(flag.Flag));
	}

	CompilerOptions::CompilerOptions(int argc, char** argv)
		: WorkingDir(std::filesystem::current_path())
	{
		ParseArgs(argc, argv);
	}

	std::vector<std::string> CompilerOptions::Get(CompilerFlag::Type type) const
	{
		std::vector<std::string> result;
		result.reserve(Flags.size());

		for (const auto& element : Flags)
		{
			if (element.Flag == type && element.Value.has_value())
				result.push_back(element.Value.value());
		}

		return result;
	}

	void CompilerOptions::ParseArgs(int argc, char** argv)
	{
		for (int i = 1; i < argc; i++)
		{
			auto flag = GetFlag(std::string(argv[i]));
			if (!flag.has_value())
				continue;

			Flags.push_back(flag.value());
		}
	}

}

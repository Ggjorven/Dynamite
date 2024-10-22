#include "dypch.h"
#include "CompilerOptions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

using namespace Pulse::Enum::Bitwise;

namespace Dynamite
{

	namespace
	{
		std::string& StrLower(std::string& val)
		{
			for (char& c : val)
				c = std::tolower(c);

			return val;
		}
	}



	CompilerOptions::CompilerOptions(int argc, char** argv)
		: Argc(argc), Argv(argv), WorkingDir(std::filesystem::current_path()), OutputDir(OutputDirectory)
	{
		ParseArgs(argc, argv);
	}

	const bool CompilerOptions::Contains(CompilerFlag type) const
	{
		return static_cast<bool>((Flags & type));
	}

	void CompilerOptions::ParseArgs(int argc, char** argv)
	{
		for (int i = 1; i < argc; i++)
		{
			std::string flag = argv[i];

			// After '--' value
			std::string flagName = {};
			if (flag.size() > 2)
				flagName = flag.substr(2, flag.size() - 2);

			// After '--X=' flag value
			std::string value = {};
			if (flag.size() > 4)
				value = flag.substr(4, flag.size() - 4);

			///////////////////////////////////
			// Files/Directories
			///////////////////////////////////
			// If not '--' it's a file.
			if (flag.substr(0, 2) != "--")
				Files.push_back(flag);

			// IncludeDir
			else if (flag.substr(2, 2) == "I=")
				IncludeDirs.push_back(value);

			// OutputDir
			else if (flag.substr(2, 2) == "O=")
				OutputDir = value;

			///////////////////////////////////
			// Flags
			///////////////////////////////////
			else if (StrLower(flagName) == "verbose")
				Flags |= CompilerFlag::Verbose;
		}
	}

}
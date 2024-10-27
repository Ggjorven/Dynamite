#include "dypch.h"
#include "CompilerOptions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

using namespace Pulse::Enum::Bitwise;

namespace Dynamite
{

	CompilerOptions::CompilerOptions(int argc, char** argv)
		: Argc(argc), Argv(argv), WorkingDir(std::filesystem::current_path()), OutputDir(OutputDirectory)
	{
		ParseArgs(argc, argv);
	}

	bool CompilerOptions::Contains(CompilerFlag type) const
	{
		return static_cast<bool>((Flags & type));
	}

	void CompilerOptions::PrintHelpMenu()
	{
		DY_LOG_TRACE("-----------------------------");
		DY_LOG_TRACE("-- Dynamite Help Menu");
		DY_LOG_TRACE("-----------------------------");
		DY_LOG_TRACE("");
		DY_LOG_TRACE(" (-) <filename> ");
		DY_LOG_TRACE("     Adds a file to units which will be compiled.");
		DY_LOG_TRACE("");
		DY_LOG_TRACE("");
		DY_LOG_TRACE(" (-) --I=<includedir> ");
		DY_LOG_TRACE("     Adds an include directory for all compilation units.");
		DY_LOG_TRACE("");
		DY_LOG_TRACE(" (-) --O=<outputdir> ");
		DY_LOG_TRACE("     Sets the output directory for all intermediate and");
		DY_LOG_TRACE("     primary binary/result files.");
		DY_LOG_TRACE("");
		DY_LOG_TRACE("");
		DY_LOG_TRACE(" (-) --verbose ");
		DY_LOG_TRACE("     Sets the logging level to 'verbose' which prints");
		DY_LOG_TRACE("     a lot more debug information.");
		DY_LOG_TRACE("");
		DY_LOG_TRACE(" (-) --help ");
		DY_LOG_TRACE("     Prints this help menu.");

		DY_LOG_TRACE("");
		DY_LOG_TRACE("-----------------------------");
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
			std::string flagValue = {};
			if (flag.size() > 4)
				flagValue = flag.substr(4, flag.size() - 4);

			///////////////////////////////////
			// Files/Directories
			///////////////////////////////////
			// If not '--' it's a file.
			if (flag.substr(0, 2) != "--")
				Files.push_back(flag);

			// IncludeDir
			else if (flag.substr(2, 2) == "I=")
				IncludeDirs.push_back(flagValue);

			// OutputDir
			else if (flag.substr(2, 2) == "O=")
				OutputDir = flagValue;

			///////////////////////////////////
			// Flags
			///////////////////////////////////
			else if (Utils::StrLower(flagName) == "verbose")
				Flags |= CompilerFlag::Verbose;

			///////////////////////////////////
			// Other
			///////////////////////////////////
			else if (Utils::StrLower(flagName) == "help")
			{
				PrintHelpMenu();

				Files.clear();
				IncludeDirs.clear();
				Flags = CompilerFlag::None;

				Quit = true;

				return;
			}

			else
				DY_LOG_ERROR("Invalid compiler option: '{0}'. \n    Use '--help' to show all options.", flag);
		}
	}

}
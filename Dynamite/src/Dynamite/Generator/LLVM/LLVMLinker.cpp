#include "dypch.h"
#include "LLVMLinker.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerSuite.hpp"

#include <lld/Common/Driver.h>

#if defined(DY_PLATFORM_WINDOWS)
	LLD_HAS_DRIVER(coff)
#elif defined(DY_PLATFORM_LINUX) // Unsupported.
	LLD_HAS_DRIVER(elf)
#endif

namespace Dynamite
{

        void LLVMLinker::Link(const std::filesystem::path& outputDir, const std::vector<std::filesystem::path>& objectFiles)
        {
    #if defined(DY_PLATFORM_WINDOWS)
            std::vector<const char*> args;
            std::vector<std::string> stringRefs;
            stringRefs.reserve(1 + objectFiles.size());
            args.reserve(2 + objectFiles.size() + 1);
            
            args.push_back("lld");

            stringRefs.push_back(std::string("/out:") + std::filesystem::path(outputDir / "output.exe").string());
            args.push_back(stringRefs.back().c_str());

            for (const auto& file : objectFiles)
            {
                stringRefs.push_back(file.string());
                args.push_back(stringRefs.back().c_str());
            }

            args.push_back("libcmt.lib");

            if (!lld::coff::link(args, llvm::outs(), llvm::errs(), false, false))
            {
                CompilerSuite::Get().Error("Linking failed!");
                return;
            }

    #elif defined(DY_PLATFORM_LINUX) // Unsupported.
        // TODO: Linux support
		DY_ASSERT(0, "LINKING NOT SUPPORTED ON LINUX.");
    #endif  

        DY_LOG_TRACE("Linking successful!");
	}

}
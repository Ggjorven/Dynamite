#include "dypch.h"
#include "Linker.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include <lld/Common/Driver.h>

#if defined(DY_PLATFORM_WINDOWS)
    LLD_HAS_DRIVER(coff)
#elif defined(DY_PLATFORM_LINUX)
    LLD_HAS_DRIVER(elf)
#endif

namespace Dynamite
{

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    void Linker::Link(const std::filesystem::path& outputDir, const std::vector<std::filesystem::path>& objectFiles)
    {
    #if defined(DY_PLATFORM_WINDOWS)
        std::vector<const char*> args;
        args.reserve(3 + objectFiles.size() + 1);

        std::vector<std::string> stringRefs;
        stringRefs.reserve(1 + objectFiles.size());

        // Add all arguments
        {
            args.push_back("lld");

            stringRefs.push_back("/out:" + (outputDir / "output.exe").string());
            args.push_back(stringRefs.back().c_str());

            // Enable debug information to emit a .pdb file
            if (Compiler::GetOptions().Contains(CompilerFlag::Debug))
                args.push_back("/debug");

            // Add object files
            for (const auto& file : objectFiles) 
            {
                stringRefs.push_back(file.string());
                args.push_back(stringRefs.back().c_str());
            }

            // Link static runtime library
            args.push_back("libcmt.lib");
        }

        if (!lld::coff::link(args, llvm::outs(), llvm::errs(), false, false))
        {
            Compiler::Error("Linking failed!");
            return;
        }

        DY_LOG_INFO("Application linkage succesful.");

    #elif defined(DY_PLATFORM_LINUX)
        DY_LOG_ERROR("Linking is currently not support on linux");
    #endif
    }

}
#include "dypch.h"
#include "ObjectPass.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/MC/TargetRegistry.h>

namespace Dynamite
{

    namespace
    {
        std::string GetLLVMFeatures()
        {
            llvm::StringMap<bool> features = llvm::sys::getHostCPUFeatures();

            std::string result = {};
            for (auto& feature : features)
            {
                if (feature.second)
                    result += "+" + feature.first().str() + ",";
            }

            return result;
        }
    }

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	void ObjectPass::Run(llvm::Module& mod, const std::filesystem::path& objectPath)
	{
        // Get the target triple
        std::string targetTriple = llvm::sys::getProcessTriple();
        mod.setTargetTriple(targetTriple);

        std::string error = {};
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
        if (!target)
        {
            Compiler::Error("Failed to find target, error: {0}", error);
            return;
        }

        // Create target machine
        llvm::TargetOptions opt = {};
        llvm::TargetMachine* targetMachine = target->createTargetMachine(targetTriple, llvm::sys::getHostCPUName(), GetLLVMFeatures(), opt, std::optional<llvm::Reloc::Model>());

        mod.setDataLayout(targetMachine->createDataLayout());

        // Create the output .o file
        std::error_code errorCode = {};
        llvm::raw_fd_ostream dest(objectPath.string(), errorCode, llvm::sys::fs::OF_None);

        if (errorCode)
        {
            Compiler::Error("Could not open file: '{0}'\n    Error: {1}", objectPath.string(), errorCode.message());
            return;
        }

        // Emit object file
        llvm::legacy::PassManager pass = {};
        llvm::CodeGenFileType fileType = llvm::CodeGenFileType::ObjectFile;

        if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
        {
            Compiler::Error("TargetMachine can't emit a file of this type");
            return;
        }

        pass.run(mod);
        dest.flush();

        DY_LOG_TRACE("Object file '{0}' has been generated.", objectPath.string());
	}

}
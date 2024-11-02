#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/IR/IRPass.hpp"

#include <llvm/ADT/StringMap.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>

namespace Dynamite
{

    using namespace Language;

	/////////////////////////////////////////////////////////////////
	// Object pass
	/////////////////////////////////////////////////////////////////
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

    static void OutputObjectFile(llvm::Module& mod, const std::filesystem::path& objectPath)
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

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Generator::Generator(Language::Node::Program& program, std::string& generatedIR)
		: m_Program(program), m_GeneratedIR(generatedIR)
	{
	}

	void Generator::Generate(const std::filesystem::path& outputPath)
	{
		// Initialization
		llvm::InitLLVM LLVMLifeSpan(Compiler::Get().GetOptions().Argc, Compiler::Get().GetOptions().Argv);

		llvm::LLVMContext context;
		llvm::Module mod(outputPath.filename().string(), context);
		//mod.addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);

		// Generate IR
        {
		    IRPass irPass(m_GeneratedIR);
            irPass.Generate(m_Program, context, mod);
        }

		// Initialize Target specific functionality.
        {
		    llvm::InitializeNativeTarget();
		    llvm::InitializeNativeTargetAsmPrinter();
		    llvm::InitializeNativeTargetAsmParser();
        }

		// Object passes
        {
		    std::filesystem::path objectPath = outputPath.parent_path() / CompilerOptions::IntermediateDirectory / outputPath.filename().replace_extension(".o");
		    if (!std::filesystem::exists(objectPath))
			    std::filesystem::create_directories(objectPath.parent_path());

            OutputObjectFile(mod, objectPath);
        }
	}

}
#include "dypch.h"
#include "LLVMGenerator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerOptions.hpp"
#include "Dynamite/Compiler/CompilerSuite.hpp"

#include "Dynamite/Generator/LLVM/LLVMIRGenerator.hpp"
#include "Dynamite/Generator/LLVM/LLVMObjectPasses.hpp"
#include "Dynamite/Generator/LLVM/LLVMLinker.hpp"

#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/TargetSelect.h>

namespace Dynamite
{

	void LLVMGenerator::Generate(Node::Program& program, const std::filesystem::path& outputPath)
	{
		// Initialization
		llvm::InitLLVM LLVMLifeSpan(CompilerSuite::Get().GetOptions().Argc, CompilerSuite::Get().GetOptions().Argv);

		llvm::LLVMContext context;
		llvm::Module mod(outputPath.filename().string(), context);
		//mod.addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);

		// Generate IR
		LLVMIRGenerator irGenerator;
		irGenerator.Generate(program, context, mod);

		// Initialize Target specific things.
		llvm::InitializeNativeTarget();
		llvm::InitializeNativeTargetAsmPrinter();
		llvm::InitializeNativeTargetAsmParser();

		// Object passes
		LLVMObjectPasses objectPasses;
		
		std::filesystem::path objectPath = outputPath.parent_path() / CompilerOptions::IntermediatePath / outputPath.filename().replace_extension(".o");
		if (!std::filesystem::exists(objectPath))
			std::filesystem::create_directories(objectPath.parent_path());
		
		objectPasses.Run(mod, objectPath);
	}

}
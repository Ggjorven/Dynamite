#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/IR/IRPass.hpp"
#include "Dynamite/Generator/Main/ObjectPass.hpp"

#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/TargetSelect.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Generator::Generator(Node::Program& program)
		: m_Program(program)
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
		IRPass irGenerator;
		irGenerator.Generate(m_Program, context, mod);

		// Initialize Target specific functionality.
		llvm::InitializeNativeTarget();
		llvm::InitializeNativeTargetAsmPrinter();
		llvm::InitializeNativeTargetAsmParser();

		// Object passes
		ObjectPass objectPass;

		std::filesystem::path objectPath = outputPath.parent_path() / CompilerOptions::IntermediateDirectory / outputPath.filename().replace_extension(".o");
		if (!std::filesystem::exists(objectPath))
			std::filesystem::create_directories(objectPath.parent_path());

		objectPass.Run(mod, objectPath);
	}

}
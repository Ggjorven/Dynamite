#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

// #include <llvm/ADT/StringMap.h>
// #include <llvm/IR/Module.h>
// #include <llvm/IR/LegacyPassManager.h>
// #include <llvm/Target/TargetMachine.h>
// #include <llvm/TargetParser/Host.h>
// #include <llvm/MC/TargetRegistry.h>
// #include <llvm/Support/InitLLVM.h>
// #include <llvm/Support/TargetSelect.h>
// #include <llvm/Support/FileSystem.h>
// #include <llvm/Support/InitLLVM.h>
// #include <llvm/Support/raw_ostream.h>
// #include <llvm/Support/TargetSelect.h>

namespace Dynamite
{

    using namespace Language;

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Generator::Generator(Language::Node::Program& program, std::string& generatedIR)
		: m_Program(program), m_GeneratedIR(generatedIR)
	{
	}

	void Generator::Generate(const std::filesystem::path& outputPath)
	{
        DY_LOG_ERROR("Generated is currently disabled.");
	}

}
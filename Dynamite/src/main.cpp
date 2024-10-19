#include "dypch.h"

/*
#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerSuite.hpp"

#include <fstream>
#include <sstream>

using namespace Dynamite;

int main(int argc, char* argv[])
{
	Logger::Init();

	CompilerSuite compilerSuite(CompilerOptions(argc, argv));
	compilerSuite.Compile();

	return 0;
}
*/

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

//#include <llvm/Support/Dwarf.h>
//#include <llvm/Support/Triple.h>

int main() 
{
    // Initialize LLVM components
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::LLVMContext context;

    // Create a new module
    std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("test_module", context);

    // Create a function signature (int32_t foo())
    llvm::FunctionType* functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    llvm::Function* mainFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "main", module.get());

    // Create a basic block and set an IR builder to insert instructions
    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, "entry", mainFunction);
    llvm::IRBuilder<> builder(entryBlock);

    // Return a constant value (e.g., return 42;)
    builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 42));

    // Verify the generated module
    if (llvm::verifyFunction(*mainFunction, &llvm::errs())) {
        llvm::errs() << "Function verification failed!\n";
        return 1;
    }

    // Print the LLVM IR to stdout
    module->print(llvm::outs(), nullptr);

    return 0;
}
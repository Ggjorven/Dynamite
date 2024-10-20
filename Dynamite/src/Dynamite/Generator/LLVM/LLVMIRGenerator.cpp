#include "dypch.h"
#include "LLVMIRGenerator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerSuite.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LegacyPassManager.h>

namespace Dynamite
{

	void LLVMIRGenerator::Generate(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod)
	{
        // Main function
        {
            // Create the main function: i32 main()
            llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
            llvm::Function* mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", mod);

            // Create the entry basic block for the main function
            llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
            llvm::IRBuilder<> builder(entry);
            
            // Create actual instructions
            builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 23));
        }

		// Verification of module.
		if (llvm::verifyModule(mod, &llvm::errs()))
			CompilerSuite::Get().Error("Module verification failed.");
	}

}
#include "dypch.h"
#include "IRPass.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/TypeRetrieval.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>

namespace Dynamite
{

	void IRPass::Generate(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod)
	{
        std::unordered_map<std::string, llvm::FunctionCallee> s_Functions = { };

        llvm::IRBuilder<> builder(context);
        /////////////////////////////////////////////////////////////////
        // LibC functions
        /////////////////////////////////////////////////////////////////
        {
            // malloc function (void* malloc(size_t))
            {
                llvm::Type* voidPtrTy = llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0);
                llvm::Type* sizeTy = llvm::Type::getInt64Ty(context);

                // Define the malloc function
                std::vector<llvm::Type*> mallocArgs = { sizeTy };
                llvm::FunctionType* mallocType = llvm::FunctionType::get(voidPtrTy, mallocArgs, false);

                llvm::FunctionCallee mallocFunc = mod.getOrInsertFunction("malloc", mallocType);
                s_Functions.insert({ "malloc", mallocFunc });
            }

            // free function (void free(void*))
            {
                llvm::Type* voidTy = llvm::Type::getVoidTy(context);
                llvm::Type* voidPtrTy = llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0);

                std::vector<llvm::Type*> freeArgs = { voidPtrTy };
                llvm::FunctionType* freeType = llvm::FunctionType::get(voidTy, freeArgs, false);

                llvm::FunctionCallee freeFunc = mod.getOrInsertFunction("free", freeType);
                s_Functions.insert({ "free", freeFunc });
            }

            // printf function (i32 printf(char*, ...))
            {
                llvm::Type* int32Ty = llvm::Type::getInt32Ty(context);
                llvm::Type* charPtrTy = llvm::Type::getInt8Ty(context)->getPointerTo();

                std::vector<llvm::Type*> printfArgs = { charPtrTy };  // printf takes a char* as the first argument
                llvm::FunctionType* printfType = llvm::FunctionType::get(int32Ty, printfArgs, true); // true means printf has varargs

                llvm::FunctionCallee printfFunc = mod.getOrInsertFunction("printf", printfType);
                s_Functions.insert({ "printf", printfFunc });
            }

            // strlen function (size_t strlen(const char*))
            {
                llvm::Type* charPtrTy = llvm::Type::getInt8Ty(context)->getPointerTo();
                llvm::Type* sizeTy = llvm::Type::getInt64Ty(context);  // size_t is typically an unsigned 64-bit integer on most platforms

                std::vector<llvm::Type*> strlenArgs = { charPtrTy };  // strlen takes a char* as the only argument
                llvm::FunctionType* strlenType = llvm::FunctionType::get(sizeTy, strlenArgs, false); // No varargs, just char*

                llvm::FunctionCallee strlenFunc = mod.getOrInsertFunction("strlen", strlenType);
                s_Functions.insert({ "strlen", strlenFunc });
            }

            // strcat function (char* strcat(char*, const char*))
            {
                llvm::Type* charPtrTy = llvm::Type::getInt8Ty(context)->getPointerTo();

                std::vector<llvm::Type*> strcatArgs = { charPtrTy, charPtrTy };  // strcat takes two char* arguments
                llvm::FunctionType* strcatType = llvm::FunctionType::get(charPtrTy, strcatArgs, false); // No varargs

                llvm::FunctionCallee strcatFunc = mod.getOrInsertFunction("strcat", strcatType);
                s_Functions.insert({ "strcat", strcatFunc });
            }
        }

        /////////////////////////////////////////////////////////////////
        // Custom functions
        /////////////////////////////////////////////////////////////////
        {
            // Main function 
            {
                // Create the main function: i32 main(i32 argc, i8** argv)
                llvm::Type* int32Ty = llvm::Type::getInt32Ty(context);
                llvm::Type* int8PtrPtrTy = llvm::Type::getInt8Ty(context)->getPointerTo()->getPointerTo();

                // Function signature: int (i32, i8**)
                std::vector<llvm::Type*> mainFuncArgs = { int32Ty, int8PtrPtrTy };
                llvm::FunctionType* mainFuncType = llvm::FunctionType::get(int32Ty, mainFuncArgs, false);

                llvm::FunctionCallee mainFunc = mod.getOrInsertFunction("main", mainFuncType);
                llvm::Function* mainFuncFunc = llvm::cast<llvm::Function>(mainFunc.getCallee());
                s_Functions.insert({ "main", mainFunc });

                // Assign names to the function arguments
                llvm::Function::arg_iterator args = mainFuncFunc->arg_begin();
                llvm::Value* argc = args++;
                argc->setName("argc");

                llvm::Value* argv = args++;
                argv->setName("argv");

                // Create the entry basic block
                llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFuncFunc);
                builder.SetInsertPoint(entry);

                // Create a loop to iterate through argv and print each argument
                llvm::BasicBlock* loopCondBB = llvm::BasicBlock::Create(context, "loopCond", mainFuncFunc);
                llvm::BasicBlock* loopBodyBB = llvm::BasicBlock::Create(context, "loopBody", mainFuncFunc);
                llvm::BasicBlock* loopEndBB = llvm::BasicBlock::Create(context, "loopEnd", mainFuncFunc);

                // Initialize index variable (i = 0)
                llvm::Value* zero = llvm::ConstantInt::get(int32Ty, 0);
                llvm::Value* index = builder.CreateAlloca(int32Ty, nullptr, "i");
                builder.CreateStore(zero, index);

                // Jump to the loop condition check
                builder.CreateBr(loopCondBB);

                // Condition check: i < argc
                builder.SetInsertPoint(loopCondBB);
                llvm::Value* iVal = builder.CreateLoad(int32Ty, index);
                llvm::Value* cond = builder.CreateICmpSLT(iVal, argc);
                builder.CreateCondBr(cond, loopBodyBB, loopEndBB);

                // Loop body: printf("%s\n", argv[i])
                builder.SetInsertPoint(loopBodyBB);

                // Assuming 'argv' is a pointer to a pointer of type int8*
                llvm::Type* int8PtrTy = llvm::Type::getInt8Ty(context)->getPointerTo();

                // Step 1: Use CreateGEP to access argv[i]
                llvm::Value* argvIndex = builder.CreateGEP(int8PtrTy, argv, iVal);  // argv[i] equivalent

                // Step 2: Load the value from argv[i], which is a char* (i8*)
                llvm::Value* argStr = builder.CreateLoad(int8PtrTy, argvIndex);  // Load argv[i] as i8*

                // Create format string "%s\n" for printing each argument
                llvm::Value* formatStr = builder.CreateGlobalStringPtr("%s\n");

                // Call printf with the current argument
                builder.CreateCall(s_Functions["printf"], { formatStr, argStr });

                // Increment i
                llvm::Value* one = llvm::ConstantInt::get(int32Ty, 1);
                llvm::Value* nextIVal = builder.CreateAdd(iVal, one);
                builder.CreateStore(nextIVal, index);

                // Jump back to the loop condition
                builder.CreateBr(loopCondBB);

                // End of the loop
                builder.SetInsertPoint(loopEndBB);

                // Return arg count
                builder.CreateRet(argc);
            }
        }

        // Verification of module.
        if (llvm::verifyModule(mod, &llvm::errs()))
            Compiler::Error("Module verification failed.");
	}

}
#include "dypch.h"
#include "IRPass.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRFunctions.hpp"
#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Support/raw_ostream.h>

namespace Dynamite
{

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    void IRPass::Generate(Node::Program& program, llvm::LLVMContext& context, llvm::Module& mod)
    {
        llvm::IRBuilder<> builder(context);

        /////////////////////////////////////////////////////////////////
        // Definitions
        /////////////////////////////////////////////////////////////////
        for (const auto& definition : program.Definitions)
        {
            struct DefineVisitor
            {
                llvm::LLVMContext& Context;
                llvm::IRBuilder<>& Builder;
                llvm::Module& Module;

                void operator () (const Node::Reference<Node::FunctionDeclaration> obj) const
                {
                    IRFunctions::GenDeclaration(obj, Context, Builder, Module);
                }
                void operator () (const Node::Reference<Node::FunctionDefinition> obj) const
                {
                    IRFunctions::GenDefinition(obj, Context, Builder, Module);
                }
                void operator () (const Node::Reference<Node::VariableStatement> obj) const
                {
                    DY_ASSERT(0, "TODO");
                }
            };

            std::visit(DefineVisitor(context, builder, mod), definition);
        }

        if (Compiler::GetOptions().Contains(CompilerFlag::Verbose))
            mod.print(llvm::outs(), nullptr, false, true);

        // Verification of module.
        if (llvm::verifyModule(mod, &llvm::errs()))
            Compiler::Error("Module verification failed.");
    }

}

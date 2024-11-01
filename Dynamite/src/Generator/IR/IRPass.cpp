#include "dypch.h"
#include "IRPass.hpp"

#include "Core/Logging.hpp"

#include "Compiler/Compiler.hpp"

#include "Generator/IR/Nodes/IRFunctions.hpp"
#include "Generator/IR/Nodes/IRStatements.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Support/raw_ostream.h>

namespace Dynamite::Language
{

    namespace
    {
        void GetIRString(llvm::Module& mod, std::string& str) 
        {
            llvm::raw_string_ostream irStream(str);
            mod.print(irStream, nullptr, false, true);
            irStream.flush();
        }
    }

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    IRPass::IRPass(std::string& generatedIR)
        : m_GeneratedIR(generatedIR)
    {
    }

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

                void operator () (const Node::Ref<Node::FunctionDeclaration> obj) const
                {
                    IRFunctions::GenDeclaration(obj, Context, Builder, Module);
                }
                void operator () (const Node::Ref<Node::FunctionDefinition> obj) const
                {
                    IRFunctions::GenDefinition(obj, Context, Builder, Module);
                }
                void operator () (const Node::Ref<Node::VariableStatement> obj) const
                {
                    DY_ASSERT(0, "TODO");
                }
            };

            std::visit(DefineVisitor(context, builder, mod), definition);
        }

        // Retrieve IR
        GetIRString(mod, m_GeneratedIR);

        // Verification of module.
        if (llvm::verifyModule(mod, &llvm::errs()))
            Compiler::Error("Module verification failed.");
    }

}

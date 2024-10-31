#include "dypch.h"
#include "IRTerms.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"
#include "Dynamite/Generator/IR/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Terms
	/////////////////////////////////////////////////////////////////
	llvm::Value* IRTerms::GenTerm(const Node::Reference<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		struct TermVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			Optional<Type> EnforceType;

			llvm::Value* operator () (const Node::Reference<Node::LiteralTerm> obj) const
			{
				return GenLiteral(obj, Context, Builder, Module, EnforceType);
			}
			llvm::Value* operator () (const Node::Reference<Node::IdentifierTerm> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
			llvm::Value* operator () (const Node::Reference<Node::ParenthesisTerm> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
		};

		return std::visit(TermVisitor(context, builder, mod, enforceType), term->Term);
	}

	llvm::Value* IRTerms::GenLiteral(const Node::Reference<Node::LiteralTerm> lit, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		if (enforceType.HasValue())
			return GeneratorTypes::GetValue(context, enforceType.Value(), lit->Literal.Value).LLVMValue;
		
		return GeneratorTypes::GetValue(context, lit->GetType(), lit->Literal.Value).LLVMValue;
	}

}

#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace Dynamite
{

	class IRTerms
	{
	public:
		static llvm::Value* GenTerm(const Node::Reference<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});

		// Reference<LiteralTerm>, Reference<IdentifierTerm>, Reference<ParenthesisTerm>>;
		static llvm::Value* GenLiteral(const Node::Reference<Node::LiteralTerm> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});
	};

}
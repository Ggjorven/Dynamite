#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Program.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace Dynamite::Language
{

	class IRTerms
	{
	public:
		static llvm::Value* GenTerm(const Node::Ref<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});

		static llvm::Value* GenLiteral(const Node::Ref<Node::LiteralTerm> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});
		static llvm::Value* GenIdentifier(const Node::Ref<Node::IdentifierTerm> identifier, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});
	};

}
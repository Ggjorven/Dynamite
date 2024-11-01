#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Program.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace Dynamite::Language
{

	class IRExpressions
	{
	public:
		static llvm::Value* GenExpression(const Node::Ref<Node::Expression> expr, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});

		static llvm::Value* GenTerm(const Node::Ref<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});
		static llvm::Value* GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	};

}
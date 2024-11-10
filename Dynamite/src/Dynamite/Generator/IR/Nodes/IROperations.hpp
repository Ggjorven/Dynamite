#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

namespace Dynamite::Language
{

	class IROperations
	{
	public:
		static llvm::Value* GenOperation(const Node::Ref<Node::BinaryExpr> expr, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);

		static llvm::Value* GenAddition(const Node::Ref<Node::BinaryAddition> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenSubtraction(const Node::Ref<Node::BinarySubtraction> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenMultiplication(const Node::Ref<Node::BinaryMultiplication> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenDivision(const Node::Ref<Node::BinaryDivision> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenOR(const Node::Ref<Node::BinaryOR> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenAND(const Node::Ref<Node::BinaryAND> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenXOR(const Node::Ref<Node::BinaryXOR> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	};

}
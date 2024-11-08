#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>

namespace Dynamite::Language
{

	class IRBranches
	{
	public:
		static void GenBranch(const Node::Ref<Node::ConditionBranch> branch, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, llvm::BasicBlock* branchBlock, llvm::BasicBlock* mergeBlock);
		
		static void GenElseIf(const Node::Ref<Node::ElseIfBranch> elseIfBranch, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, llvm::BasicBlock* elseIfBlock, llvm::BasicBlock* mergeBlock);
		static void GenElse(const Node::Ref<Node::ElseBranch> elseBranch, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, llvm::BasicBlock* elseBlock, llvm::BasicBlock* mergeBlock);
	};

}
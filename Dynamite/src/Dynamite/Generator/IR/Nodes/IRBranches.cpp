#include "dypch.h"
#include "IRBranches.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRTerms.hpp"
#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"
#include "Dynamite/Generator/IR/Nodes/IRFunctions.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include "Dynamite/Generator/IR/Collections/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Branches
	/////////////////////////////////////////////////////////////////
	void IRBranches::GenBranch(const Node::Ref<Node::ConditionBranch> branch, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, llvm::BasicBlock* branchBlock, llvm::BasicBlock* mergeBlock)
	{
		struct BranchVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			llvm::BasicBlock* BranchBlock;
			llvm::BasicBlock* MergeBlock;

			void operator () (const Node::Ref<Node::ElseIfBranch> obj)
			{
				GenElseIf(obj, Context, Builder, Module, BranchBlock, MergeBlock);
			}
			void operator () (const Node::Ref<Node::ElseBranch> obj)
			{
				GenElse(obj, Context, Builder, Module, BranchBlock, MergeBlock);
			}
		};

		std::visit(BranchVisitor(context, builder, mod, branchBlock, mergeBlock), branch->Branch);
	}

	void IRBranches::GenElseIf(const Node::Ref<Node::ElseIfBranch> elseIfBranch, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, llvm::BasicBlock* elseIfBlock, llvm::BasicBlock* mergeBlock)
	{
		llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();
		llvm::BasicBlock* implBlock = llvm::BasicBlock::Create(context, "nextImpl.block", parentFunction);
		llvm::BasicBlock* nextBlock = elseIfBranch->Next.HasValue() ? llvm::BasicBlock::Create(context, "next.block", parentFunction) : mergeBlock;

		llvm::Value* expr = IRExpressions::GenExpression(elseIfBranch->Expr, context, builder, mod, Type(TypeSpecifier::Bool));

		// Conditional branch based on `elseIfBranch` condition
		builder.SetInsertPoint(elseIfBlock);
		builder.CreateCondBr(expr, implBlock, nextBlock);

		// Populate the "else if" block
		builder.SetInsertPoint(implBlock);
		IRStatements::GenScope(elseIfBranch->Scope, context, builder, mod);
		builder.CreateBr(mergeBlock);

		// Generate following branches if any
		if (elseIfBranch->Next.HasValue()) 
			IRBranches::GenBranch(elseIfBranch->Next.Value(), context, builder, mod, nextBlock, mergeBlock);
	}

	void IRBranches::GenElse(const Node::Ref<Node::ElseBranch> elseBranch, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, llvm::BasicBlock* elseBlock, llvm::BasicBlock* mergeBlock)
	{
		builder.SetInsertPoint(elseBlock);
		IRStatements::GenScope(elseBranch->Scope, context, builder, mod);
		builder.CreateBr(mergeBlock);
	}

}

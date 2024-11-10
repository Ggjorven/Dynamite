#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

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
		static llvm::Value* GenBinary(const Node::Ref<Node::BinaryExpr> binary, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType = {});
		static llvm::Value* GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenReference(const Node::Ref<Node::ReferenceExpr> reference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenAddress(const Node::Ref<Node::AddressExpr> address, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static llvm::Value* GenDereference(const Node::Ref<Node::DereferenceExpr> dereference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	};

}
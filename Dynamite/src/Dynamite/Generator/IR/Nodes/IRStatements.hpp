#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

namespace Dynamite
{

	class IRStatements
	{
	public:
		static void GenStatement(const Node::Reference<Node::Statement> statement, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		
		static void GenScope(const Node::Reference<Node::ScopeStatement> scope, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static void GenReturn(const Node::Reference<Node::ReturnStatement> ret, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		// TODO: Function call
	};

}
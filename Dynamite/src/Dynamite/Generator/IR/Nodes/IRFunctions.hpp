#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

namespace Dynamite
{

	class IRFunctions
	{
	public:
		static void GenDeclaration(const Node::Reference<Node::FunctionDeclaration> declaration, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static void GenDefinition(const Node::Reference<Node::FunctionDefinition> definition, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	
		static llvm::Value* GenFunctionCall(const Node::Reference<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	};

}
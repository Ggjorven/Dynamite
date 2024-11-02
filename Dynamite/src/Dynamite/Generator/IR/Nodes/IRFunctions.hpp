#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

namespace Dynamite::Language
{

	class IRFunctions
	{
	public:
		static void GenDeclaration(const Node::Ref<Node::FunctionDeclaration> declaration, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static void GenDefinition(const Node::Ref<Node::FunctionDefinition> definition, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	
		static llvm::Value* GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	};

}
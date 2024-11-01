#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Program.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

namespace Dynamite::Language
{

	class IRStatements
	{
	public:
		static void GenStatement(const Node::Ref<Node::Statement> statement, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		
		static void GenVariable(const Node::Ref<Node::VariableStatement> var, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static void GenScope(const Node::Ref<Node::ScopeStatement> scope, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, bool startScope = true);
		static void GenReturn(const Node::Ref<Node::ReturnStatement> ret, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
		static void GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod);
	};

}
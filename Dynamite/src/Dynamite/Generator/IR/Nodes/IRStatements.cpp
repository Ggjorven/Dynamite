#include "dypch.h"
#include "IRStatements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

#include "Dynamite/Generator/IR/IRState.hpp"
#include "Dynamite/Generator/IR/IRFunctionCollection.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/IntrinsicInst.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Statements
	/////////////////////////////////////////////////////////////////
	void IRStatements::GenStatement(const Node::Reference<Node::Statement> statement, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		struct StatementVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			void operator () (const Node::Reference<Node::IfStatement> obj) const
			{
				DY_ASSERT(0, "TODO");
			}
			void operator () (const Node::Reference<Node::VariableStatement> obj) const
			{
				DY_ASSERT(0, "TODO");
			}
			void operator () (const Node::Reference<Node::ScopeStatement> obj) const
			{
				GenScope(obj, Context, Builder, Module);
			}
			void operator () (const Node::Reference<Node::AssignmentStatement> obj) const
			{
				DY_ASSERT(0, "TODO");
			}
			void operator () (const Node::Reference<Node::ReturnStatement> obj) const
			{
				GenReturn(obj, Context, Builder, Module);
			}
			void operator () (const Node::Reference<Node::FunctionCall> obj) const
			{
				DY_ASSERT(0, "TODO");
			}
		};

		std::visit(StatementVisitor(context, builder, mod), statement->StatementObj);
	}

	void IRStatements::GenScope(const Node::Reference<Node::ScopeStatement> scope, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		for (const auto& statement : scope->Statements)
			GenStatement(statement, context, builder, mod);
	}

	void IRStatements::GenReturn(const Node::Reference<Node::ReturnStatement> ret, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* val = IRExpressions::GenExpression(ret->Expr, context, builder, mod, IRState::CurrentFunction->GetType());
		builder.CreateRet(val);
	}

}

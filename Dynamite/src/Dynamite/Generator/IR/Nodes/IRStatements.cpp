#include "dypch.h"
#include "IRStatements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

#include "Dynamite/Generator/IR/IRState.hpp"
#include "Dynamite/Generator/IR/Nodes/IRFunctions.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include "Dynamite/Generator/IR/IRScopeCollection.hpp"
#include "Dynamite/Generator/IR/IRFunctionCollection.hpp"

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
				GenVariable(obj, Context, Builder, Module);
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
				GenFunctionCall(obj, Context, Builder, Module);
			}
		};

		std::visit(StatementVisitor(context, builder, mod), statement->StatementObj);
	}

	void IRStatements::GenVariable(const Node::Reference<Node::VariableStatement> var, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Type* varType = GeneratorTypes::GetType(context, var->GetType()).LLVMType;

		// TODO: Support arrays
		llvm::Value* variable = builder.CreateAlloca(varType, nullptr, var->Variable.Value);
		
		if (var->Expr)
		{
			llvm::Value* expr = IRExpressions::GenExpression(var->Expr, context, builder, mod, var->GetType());
			builder.CreateStore(expr, variable);
		}

		IRScopeCollection::PushVar(var->Variable.Value, var->GetType(), variable);
	}

	void IRStatements::GenScope(const Node::Reference<Node::ScopeStatement> scope, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, bool startScope)
	{
		if (startScope)
			IRScopeCollection::BeginScope();

		for (const auto& statement : scope->Statements)
			GenStatement(statement, context, builder, mod);
		
		IRScopeCollection::EndScope();
	}

	void IRStatements::GenReturn(const Node::Reference<Node::ReturnStatement> ret, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* val = IRExpressions::GenExpression(ret->Expr, context, builder, mod, IRState::CurrentFunction->GetType());
		builder.CreateRet(val);
	}

	void IRStatements::GenFunctionCall(const Node::Reference<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		IRFunctions::GenFunctionCall(funcCall, context, builder, mod);
	}

}

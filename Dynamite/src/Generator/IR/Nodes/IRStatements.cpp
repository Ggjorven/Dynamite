#include "dypch.h"
#include "IRStatements.hpp"

#include "Core/Logging.hpp"

#include "Compiler/Compiler.hpp"

#include "Generator/Types/GenTypes.hpp"

#include "Generator/IR/IRState.hpp"
#include "Generator/IR/Nodes/IRFunctions.hpp"
#include "Generator/IR/Nodes/IRExpressions.hpp"

#include "Generator/IR/Collections/IRScopeCollection.hpp"
#include "Generator/IR/Collections/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/IntrinsicInst.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Statements
	/////////////////////////////////////////////////////////////////
	void IRStatements::GenStatement(const Node::Ref<Node::Statement> statement, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		struct StatementVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			void operator () (const Node::Ref<Node::IfStatement> obj) const
			{
				DY_ASSERT(0, "TODO");
			}
			void operator () (const Node::Ref<Node::VariableStatement> obj) const
			{
				GenVariable(obj, Context, Builder, Module);
			}
			void operator () (const Node::Ref<Node::ScopeStatement> obj) const
			{
				GenScope(obj, Context, Builder, Module);
			}
			void operator () (const Node::Ref<Node::AssignmentStatement> obj) const
			{
				DY_ASSERT(0, "TODO");
			}
			void operator () (const Node::Ref<Node::ReturnStatement> obj) const
			{
				GenReturn(obj, Context, Builder, Module);
			}
			void operator () (const Node::Ref<Node::FunctionCall> obj) const
			{
				GenFunctionCall(obj, Context, Builder, Module);
			}
		};

		std::visit(StatementVisitor(context, builder, mod), statement->StatementObj);
	}

	void IRStatements::GenVariable(const Node::Ref<Node::VariableStatement> var, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Type* varType = GenTypes::GetType(context, var->GetType()).LLVMType;

		// TODO: Support arrays
		llvm::Value* variable = builder.CreateAlloca(varType, nullptr, var->Variable);
		
		if (var->Expr)
		{
			llvm::Value* expr = IRExpressions::GenExpression(var->Expr, context, builder, mod, var->GetType());
			builder.CreateStore(expr, variable);
		}

		IRScopeCollection::PushVar(var->Variable, var->GetType(), GeneratorValue(variable));
	}

	void IRStatements::GenScope(const Node::Ref<Node::ScopeStatement> scope, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, bool startScope)
	{
		if (startScope)
			IRScopeCollection::BeginScope();

		for (const auto& statement : scope->Statements)
			GenStatement(statement, context, builder, mod);
		
		IRScopeCollection::EndScope();
	}

	void IRStatements::GenReturn(const Node::Ref<Node::ReturnStatement> ret, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* val = IRExpressions::GenExpression(ret->Expr, context, builder, mod, IRState::CurrentFunction->GetType());
		builder.CreateRet(val);
	}

	void IRStatements::GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		IRFunctions::GenFunctionCall(funcCall, context, builder, mod);
	}

}

#include "dypch.h"
#include "IRStatements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/IRState.hpp"
#include "Dynamite/Generator/IR/Nodes/IRFunctions.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include "Dynamite/Generator/IR/Collections/IRScopeCollection.hpp"
#include "Dynamite/Generator/IR/Collections/IRFunctionCollection.hpp"

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
				GenAssignment(obj, Context, Builder, Module);
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

	void IRStatements::GenVariable(const Node::Ref<Node::VariableStatement> var, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<GeneratorValue> enforceValue)
	{
		llvm::Type* varType = GenTypes::GetType(context, var->GetType()).LLVMType;
		std::string allocName = var->Variable;

		llvm::Value* variable = nullptr;
		if (var->GetType().IsArray())
		{
			if (var->GetType().GetArraySize().empty())
			{
				variable = builder.CreateAlloca(varType, 0, nullptr);
			}
			else
			{
				llvm::Value* arraySize = GenTypes::GetValue(context, Type(TypeSpecifier::UInt64), var->GetType().GetArraySize()).LLVMValue;
				variable = builder.CreateAlloca(varType, 0, arraySize);
			}
		}
		else
		{
			variable = builder.CreateAlloca(varType, 0, nullptr);
		}
		variable->setName(allocName);
		
		if (enforceValue.HasValue())
		{
			builder.CreateStore(enforceValue.Value().LLVMValue, variable);
		}
		else if (var->Expr)
		{
			llvm::Value* expr = IRExpressions::GenExpression(var->Expr, context, builder, mod, var->GetType());
			builder.CreateStore(expr, variable);
		}

		IRScopeCollection::PushVar(var->Variable, var->GetType(), GeneratorValue(variable));
	}

	void IRStatements::GenAssignment(const Node::Ref<Node::AssignmentStatement> assignment, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* variable = IRScopeCollection::GetVariable(assignment->Variable).Value.LLVMValue;
		llvm::Type* varType = GenTypes::GetType(context, assignment->VariableType).LLVMType;  // Get the explicit type

		if (assignment->VariableType.IsReference() && !assignment->Expr->GetType().IsReference())
		{
			llvm::Value* expr = IRExpressions::GenExpression(assignment->Expr, context, builder, mod, assignment->VariableType.RemoveReference());
			llvm::Value* ptr = builder.CreateLoad(varType, variable, assignment->Variable);

			builder.CreateStore(expr, ptr);
		}
		else
		{
			llvm::Value* expr = IRExpressions::GenExpression(assignment->Expr, context, builder, mod, assignment->VariableType);
			builder.CreateStore(expr, variable);
		}
	}

	void IRStatements::GenScope(const Node::Ref<Node::ScopeStatement> scope, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, bool startScope, bool enforceReturn)
	{
		if (startScope)
			IRScopeCollection::BeginScope();

		for (const auto& statement : scope->Statements)
			GenStatement(statement, context, builder, mod);
		
		// No return statement at the end
		if (enforceReturn && !scope->Statements.empty() && !std::holds_alternative<Node::Ref<Node::ReturnStatement>>(scope->Statements.back()->StatementObj))
		{
			if (IRState::CurrentFunction->GetType() == Type(TypeSpecifier::Void))
				builder.CreateRetVoid();
			else
				DY_LOG_ERROR("[Internal Compiler Error] Function '{0}' does not end with return statement and return type != void.", IRState::CurrentFunction->Name);
		}

		IRScopeCollection::EndScope();
	}

	void IRStatements::GenReturn(const Node::Ref<Node::ReturnStatement> ret, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		if (ret->Expr)
		{
			llvm::Value* val = IRExpressions::GenExpression(ret->Expr, context, builder, mod, IRState::CurrentFunction->GetType());
			builder.CreateRet(val);
		}
		else
		{
			builder.CreateRetVoid();
		}
	}

	void IRStatements::GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		IRFunctions::GenFunctionCall(funcCall, context, builder, mod);
	}

}

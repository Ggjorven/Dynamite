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

		llvm::Value* variable = nullptr;
		if (var->GetType().IsArray())
		{
			if (var->GetType().GetArraySize().empty())
			{
				variable = builder.CreateAlloca(varType, 0, nullptr, var->Variable);
			}
			else
			{
				llvm::Value* arraySize = GenTypes::GetValue(context, Type(TypeSpecifier::UInt64), var->GetType().GetArraySize()).LLVMValue;
				variable = builder.CreateAlloca(varType, 0, arraySize, var->Variable);
			}
		}
		else
		{
			variable = builder.CreateAlloca(varType, 0, nullptr, var->Variable);
		}
		variable->setName(var->Variable);
		
		if (var->Expr)
		{

			/*
			llvm::Type* type = GenTypes::GetType(context, identifier->GetType()).LLVMType;

			if (enforceType.HasValue() && (identifier->GetType() != enforceType.Value()))
			{
				llvm::Value* nonCastValue = builder.CreateLoad(type, IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue);
	
				return GenTypes::Cast(builder, nonCastValue, identifier->GetType(), enforceType.Value());
			}

			return builder.CreateLoad(type, IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue);
			*/

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

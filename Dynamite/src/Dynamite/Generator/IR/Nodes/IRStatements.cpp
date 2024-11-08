#include "dypch.h"
#include "IRStatements.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/IRState.hpp"
#include "Dynamite/Generator/IR/Nodes/IRBranches.hpp"
#include "Dynamite/Generator/IR/Nodes/IRFunctions.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include "Dynamite/Generator/IR/Collections/IRScopeCollection.hpp"
#include "Dynamite/Generator/IR/Collections/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>

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
				GenIf(obj, Context, Builder, Module);
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
				llvm::Value* arraySize = GenTypes::GetLiteralValue(context, mod, Type(TypeSpecifier::UInt64), LiteralType::IntegerLiteral, var->GetType().GetArraySize()).LLVMValue;
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
		Type variableType = {};
		Type exprType = {};
		llvm::Value* variable = nullptr;
		
		// Data retrieval
		{
			// Identifier
			if (assignment->Variable->GetUnderlyingType() == Node::NodeType::IdentifierTerm)
			{
				Node::Ref<Node::IdentifierTerm> identifier = (Node::Ref<Node::IdentifierTerm>)assignment->Variable->GetUnderlying();

				variableType = identifier->IdentifierType;
				exprType = assignment->Expr->GetType();
				variable = IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue;
			}
			// Other expressions
			else
			{
				variableType = assignment->Variable->GetType();
				exprType = assignment->Expr->GetType();
				variable = IRExpressions::GenExpression(assignment->Expr, context, builder, mod, assignment->Variable->GetType());
			}
		}

		// Actual logic
		{
			llvm::Type* varType = GenTypes::GetType(context, variableType).LLVMType; // Get the explicit type

			if (variableType.IsReference() && !exprType.IsReference())
			{
				llvm::Value* expr = IRExpressions::GenExpression(assignment->Expr, context, builder, mod, variableType.RemoveReference());
				llvm::Value* ptr = builder.CreateLoad(varType, variable, variableType.IsVolatile());

				builder.CreateStore(expr, ptr);
			}
			else
			{
				llvm::Value* expr = IRExpressions::GenExpression(assignment->Expr, context, builder, mod, variableType);
				builder.CreateStore(expr, variable);
			}
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

	void IRStatements::GenIf(const Node::Ref<Node::IfStatement> ifStatement, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Function* parentFunction = builder.GetInsertBlock()->getParent();

		llvm::BasicBlock* ifBlock = llvm::BasicBlock::Create(context, "if.block", parentFunction);
		llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context, "merge.block", parentFunction);
		llvm::BasicBlock* nextBlock = ifStatement->Next.HasValue() ? llvm::BasicBlock::Create(context, "next.block", parentFunction) : mergeBlock;

		llvm::Value* expr = IRExpressions::GenExpression(ifStatement->Expr, context, builder, mod, Type(TypeSpecifier::Bool));
		builder.CreateCondBr(expr, ifBlock, nextBlock);

		// Generate the "if" branch
		builder.SetInsertPoint(ifBlock);
		IRStatements::GenScope(ifStatement->Scope, context, builder, mod);
		builder.CreateBr(mergeBlock);

		// Generate the "else if" or "else" branch, if present
		if (ifStatement->Next.HasValue()) 
			IRBranches::GenBranch(ifStatement->Next.Value(), context, builder, mod, nextBlock, mergeBlock);

		// Control flow rejoins at the merge block
		builder.SetInsertPoint(mergeBlock);
	}

}

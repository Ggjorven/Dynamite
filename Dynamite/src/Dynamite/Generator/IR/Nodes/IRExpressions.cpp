#include "dypch.h"
#include "IRExpressions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRTerms.hpp"
#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"
#include "Dynamite/Generator/IR/Nodes/IRFunctions.hpp"

#include "Dynamite/Generator/IR/Collections/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Expressions
	/////////////////////////////////////////////////////////////////
	llvm::Value* IRExpressions::GenExpression(const Node::Ref<Node::Expression> expr, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		struct ExpressionVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			Optional<Type> EnforceType;

			llvm::Value* operator () (const Node::Ref<Node::TermExpr> obj) const
			{
				return GenTerm(obj, Context, Builder, Module, EnforceType);
			}
			llvm::Value* operator () (const Node::Ref<Node::BinaryExpr> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
			llvm::Value* operator () (const Node::Ref<Node::FunctionCall> obj) const
			{
				return GenFunctionCall(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::ReferenceExpr> obj) const
			{
				return GenReference(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::AddressExpr> obj) const
			{
				return GenAddress(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::DereferenceExpr> obj) const
			{
				return GenDereference(obj, Context, Builder, Module);
			}
		};

		return std::visit(ExpressionVisitor(context, builder, mod, enforceType), expr->Expr);
	}

	llvm::Value* IRExpressions::GenTerm(const Node::Ref<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		return IRTerms::GenTerm(term, context, builder, mod, enforceType);
	}

	llvm::Value* IRExpressions::GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		return IRFunctions::GenFunctionCall(funcCall, context, builder, mod);
	}

	llvm::Value* IRExpressions::GenReference(const Node::Ref<Node::ReferenceExpr> reference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		DY_ASSERT(0, "TODO");
		return nullptr;
	}

	llvm::Value* IRExpressions::GenAddress(const Node::Ref<Node::AddressExpr> address, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		// First, generate the expression to get the base pointer
		llvm::Value* basePointer = IRExpressions::GenExpression(address->Expr, context, builder, mod);

		// You might want to handle different cases, such as if the basePointer is actually a pointer type.
		if (!basePointer->getType()->isPointerTy()) {
			// Handle the error case: the expression does not evaluate to a pointer
			DY_ASSERT(0, "Expression does not evaluate to a pointer");
			return nullptr;
		}

		// Optional: You might want to handle the case where you add an offset to the pointer
		// For example, if you want to add sizeof(uint32_t) to the base pointer
		unsigned offsetBytes = sizeof(uint32_t); // Example offset

		// Create a GEP instruction to compute the address with the offset
		//llvm::Value* offsetIndex = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), offsetBytes / llvm::PointerType::get(basePointer->getType()->getPointerElementType(), 0)-//>getPrimitiveSizeInBits() / 8);
		//llvm::Value* addressValue = builder.CreateGEP(basePointer->getType(), basePointer, offsetIndex, "address_offset");

		//return addressValue; // Return the computed address

		return nullptr;
	}


	llvm::Value* IRExpressions::GenDereference(const Node::Ref<Node::DereferenceExpr> dereference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		DY_ASSERT(0, "TODO");
		return nullptr;
	}

}

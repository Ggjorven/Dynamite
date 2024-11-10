#include "dypch.h"
#include "IROperations.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/IRState.hpp"
#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include "Dynamite/Generator/IR/Collections/IRScopeCollection.hpp"
#include "Dynamite/Generator/IR/Collections/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Operations
	/////////////////////////////////////////////////////////////////
	llvm::Value* IROperations::GenOperation(const Node::Ref<Node::BinaryExpr> expr, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		struct OperationVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			llvm::Value* operator () (const Node::Ref<Node::BinaryAddition> obj)
			{
				return GenAddition(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::BinarySubtraction> obj)
			{
				return GenSubtraction(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::BinaryMultiplication> obj)
			{
				return GenMultiplication(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::BinaryDivision> obj)
			{
				return GenDivision(obj, Context, Builder, Module);
			}

			llvm::Value* operator () (const Node::Ref<Node::BinaryOR> obj)
			{
				return GenOR(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::BinaryAND> obj)
			{
				return GenAND(obj, Context, Builder, Module);
			}
			llvm::Value* operator () (const Node::Ref<Node::BinaryXOR> obj)
			{
				return GenXOR(obj, Context, Builder, Module);
			}
		};

		return std::visit(OperationVisitor(context, builder, mod), expr->Operation);
	}

	llvm::Value* IROperations::GenAddition(const Node::Ref<Node::BinaryAddition> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* lhs = IRExpressions::GenExpression(add->LHS, context, builder, mod, add->ResultType);
		llvm::Value* rhs = IRExpressions::GenExpression(add->RHS, context, builder, mod, add->ResultType);

		Type lhsType = add->LHS->GetType();
		Type rhsType = add->RHS->GetType();

		if (lhsType.IsIntegral() && rhsType.IsIntegral())
			return builder.CreateAdd(lhs, rhs, "intAdd");
		
		else if (lhsType.IsFloat() && rhsType.IsFloat())
			return builder.CreateFAdd(lhs, rhs, "floatAdd");

		else if (lhsType.IsPointer() && rhsType.IsIntegral())
		{
			// Pointer arithmetic: add integer to pointer
			auto* pointerType = llvm::cast<llvm::PointerType>(lhs->getType());
			auto* elementType = pointerType->getNonOpaquePointerElementType();
			return builder.CreateGEP(elementType, lhs, rhs, "ptrAdd");
		}
		else if (lhsType.IsIntegral() && rhsType.IsPointer())
		{
			// Pointer arithmetic: add integer to pointer (reverse order)
			auto* pointerType = llvm::cast<llvm::PointerType>(rhs->getType());
			auto* elementType = pointerType->getNonOpaquePointerElementType();
			return builder.CreateGEP(elementType, rhs, lhs, "intPtrAdd");
		}

		DY_ASSERT(false, "[Internal Compiler Error] - Unsupported types for addition");
		return nullptr;
	}

	llvm::Value* IROperations::GenSubtraction(const Node::Ref<Node::BinarySubtraction> sub, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* lhs = IRExpressions::GenExpression(sub->LHS, context, builder, mod, sub->ResultType);
		llvm::Value* rhs = IRExpressions::GenExpression(sub->RHS, context, builder, mod, sub->ResultType);

		Type lhsType = sub->LHS->GetType();
		Type rhsType = sub->RHS->GetType();

		if (lhsType.IsIntegral() && rhsType.IsIntegral())
			return builder.CreateSub(lhs, rhs, "intSub");

		else if (lhsType.IsFloat() && rhsType.IsFloat())
			return builder.CreateFSub(lhs, rhs, "floatSub");

		else if (lhsType.IsPointer() && rhsType.IsIntegral())
		{
			// Pointer arithmetic: subtract integer from pointer
			auto* pointerType = llvm::cast<llvm::PointerType>(lhs->getType());
			auto* elementType = pointerType->getNonOpaquePointerElementType();
			llvm::Value* negRhs = builder.CreateNeg(rhs, "negInt"); // Negate integer for subtraction
			return builder.CreateGEP(elementType, lhs, negRhs, "ptrSub");
		}
		else if (lhsType.IsPointer() && rhsType.IsPointer())
		{
			// Pointer subtraction: difference in elements
			auto* elementType = llvm::cast<llvm::PointerType>(lhs->getType())->getNonOpaquePointerElementType();
			llvm::Value* ptrDiff = builder.CreatePtrDiff(elementType, lhs, rhs, "ptrDiff");
			return ptrDiff;
		}

		DY_ASSERT(false, "[Internal Compiler Error] - Unsupported types for subtraction");
		return nullptr;
	}


	llvm::Value* IROperations::GenMultiplication(const Node::Ref<Node::BinaryMultiplication> mul, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* lhs = IRExpressions::GenExpression(mul->LHS, context, builder, mod);
		llvm::Value* rhs = IRExpressions::GenExpression(mul->RHS, context, builder, mod);

		Type lhsType = mul->LHS->GetType();
		Type rhsType = mul->RHS->GetType();

		if (lhsType.IsIntegral() && rhsType.IsIntegral())
		{
			// Integer multiplication
			return builder.CreateMul(lhs, rhs, "intMul");
		}
		else if (lhsType.IsFloat() && rhsType.IsFloat())
		{
			// Floating-point multiplication
			return builder.CreateFMul(lhs, rhs, "floatMul");
		}

		DY_ASSERT(false, "[Internal Compiler Error] - Unsupported types for multiplication");
		return nullptr;
	}

	llvm::Value* IROperations::GenDivision(const Node::Ref<Node::BinaryDivision> div, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		llvm::Value* lhs = IRExpressions::GenExpression(div->LHS, context, builder, mod);
		llvm::Value* rhs = IRExpressions::GenExpression(div->RHS, context, builder, mod);

		Type resultType = div->ResultType;
		Type lhsType = div->LHS->GetType();
		Type rhsType = div->RHS->GetType();

		if (lhsType.IsIntegral() && rhsType.IsIntegral())
		{
			if (!resultType.IsUnsigned())
			{
				// Signed integer division
				return builder.CreateSDiv(lhs, rhs, "intSDiv");
			}
			else
			{
				// Unsigned integer division
				return builder.CreateUDiv(lhs, rhs, "intUDiv");
			}
		}
		else if (lhsType.IsFloat() && rhsType.IsFloat())
		{
			// Floating-point division
			return builder.CreateFDiv(lhs, rhs, "floatDiv");
		}

		DY_ASSERT(false, "[Internal Compiler Error] - Unsupported types for division");
		return nullptr;
	}


	llvm::Value* IROperations::GenOR(const Node::Ref<Node::BinaryOR> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		DY_ASSERT(false, "TODO");
		return nullptr;
	}

	llvm::Value* IROperations::GenAND(const Node::Ref<Node::BinaryAND> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		DY_ASSERT(false, "TODO");
		return nullptr;
	}

	llvm::Value* IROperations::GenXOR(const Node::Ref<Node::BinaryXOR> add, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		DY_ASSERT(false, "TODO");
		return nullptr;
	}

}

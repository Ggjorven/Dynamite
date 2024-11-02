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
			llvm::Value* operator () (const Node::Ref<Node::AddressExpr> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
			llvm::Value* operator () (const Node::Ref<Node::DereferenceExpr> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
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

}

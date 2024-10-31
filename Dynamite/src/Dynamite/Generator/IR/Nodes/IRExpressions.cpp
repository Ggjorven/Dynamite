#include "dypch.h"
#include "IRExpressions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRTerms.hpp"
#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"
#include "Dynamite/Generator/IR/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Expressions
	/////////////////////////////////////////////////////////////////
	llvm::Value* IRExpressions::GenExpression(const Node::Reference<Node::Expression> expr, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		struct ExpressionVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			Optional<Type> EnforceType;

			llvm::Value* operator () (const Node::Reference<Node::TermExpr> obj) const
			{
				return GenTerm(obj, Context, Builder, Module, EnforceType);
			}
			llvm::Value* operator () (const Node::Reference<Node::BinaryExpr> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
			llvm::Value* operator () (const Node::Reference<Node::FunctionCall> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
			llvm::Value* operator () (const Node::Reference<Node::AddressExpr> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
			llvm::Value* operator () (const Node::Reference<Node::DereferenceExpr> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
		};

		return std::visit(ExpressionVisitor(context, builder, mod, enforceType), expr->Expr);
	}

	llvm::Value* IRExpressions::GenTerm(const Node::Reference<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		return IRTerms::GenTerm(term, context, builder, mod, enforceType);
	}

}

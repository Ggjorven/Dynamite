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
		struct TargetVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			llvm::Value* operator () (const Node::Ref<Node::IdentifierTerm> obj)
			{
				return IRScopeCollection::GetVariable(obj->Identifier).Value.LLVMValue;
			}
			llvm::Value* operator () (const Node::Ref<Node::Expression> obj)
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
		};

		return std::visit(TargetVisitor(context, builder, mod), reference->Target.Target);
	}

	llvm::Value* IRExpressions::GenAddress(const Node::Ref<Node::AddressExpr> address, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		struct TargetVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			llvm::Value* operator () (const Node::Ref<Node::IdentifierTerm> obj)
			{
				return IRScopeCollection::GetVariable(obj->Identifier).Value.LLVMValue;
			}
			llvm::Value* operator () (const Node::Ref<Node::Expression> obj)
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
		};

		return std::visit(TargetVisitor(context, builder, mod), address->Target.Target);
	}


	llvm::Value* IRExpressions::GenDereference(const Node::Ref<Node::DereferenceExpr> dereference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		struct TargetVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			llvm::Value* operator () (const Node::Ref<Node::IdentifierTerm> obj)
			{
				// Pointer -> Reference, does nothing basically
				if (obj->GetType().IsPointer())
				{
					// Gives the value of the pointer to the reference
					// Since pointers and references are internally the same.
					llvm::Type* type = GenTypes::GetType(Context, obj->GetType()).LLVMType;
					return Builder.CreateLoad(type, IRScopeCollection::GetVariable(obj->Identifier).Value.LLVMValue);
				}
				else // Reference -> Value
				{
					llvm::Type* ptrType = GenTypes::GetType(Context, obj->GetType()).LLVMType;
					llvm::Value* ptr = Builder.CreateLoad(ptrType, IRScopeCollection::GetVariable(obj->Identifier).Value.LLVMValue);

					llvm::Type* valType = GenTypes::GetType(Context, obj->GetType().RemoveReference()).LLVMType;
					return Builder.CreateLoad(valType, ptr);
				}
			}
			llvm::Value* operator () (const Node::Ref<Node::Expression> obj)
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
		};

		return std::visit(TargetVisitor(context, builder, mod), dereference->Target.Target);
	}

}

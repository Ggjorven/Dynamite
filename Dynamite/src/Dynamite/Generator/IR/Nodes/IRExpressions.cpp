#include "dypch.h"
#include "IRExpressions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRTerms.hpp"
#include "Dynamite/Generator/IR/Nodes/IROperations.hpp"
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
				return GenBinary(obj, Context, Builder, Module, EnforceType);
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

	llvm::Value* IRExpressions::GenBinary(const Node::Ref<Node::BinaryExpr> binary, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		llvm::Value* value = IROperations::GenOperation(binary, context, builder, mod);

		if (enforceType.HasValue())
			return GenTypes::Cast(builder, value, binary->GetType(), enforceType.Value());

		return value;
	}

	llvm::Value* IRExpressions::GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		return IRFunctions::GenFunctionCall(funcCall, context, builder, mod);
	}

	llvm::Value* IRExpressions::GenReference(const Node::Ref<Node::ReferenceExpr> reference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		// Identifier
		if (reference->GetUnderlyingType() == Node::NodeType::IdentifierTerm)
		{
			Node::Ref<Node::IdentifierTerm> identifier = (Node::Ref<Node::IdentifierTerm>)reference->GetUnderlying();
			return IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue;
		}
		// Other expressions

		DY_ASSERT(0, "TODO");
		return nullptr;
	}

	llvm::Value* IRExpressions::GenAddress(const Node::Ref<Node::AddressExpr> address, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		// Identifier
		if (address->GetUnderlyingType() == Node::NodeType::IdentifierTerm)
		{
			Node::Ref<Node::IdentifierTerm> identifier = (Node::Ref<Node::IdentifierTerm>)address->GetUnderlying();
			return IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue;
		}
		// Other expressions

		DY_ASSERT(0, "TODO");
		return nullptr;
	}


	llvm::Value* IRExpressions::GenDereference(const Node::Ref<Node::DereferenceExpr> dereference, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		// Identifier
		if (dereference->GetUnderlyingType() == Node::NodeType::IdentifierTerm)
		{
			Node::Ref<Node::IdentifierTerm> identifier = (Node::Ref<Node::IdentifierTerm>)dereference->GetUnderlying();

			// Pointer -> Reference, does nothing internally
			if (identifier->GetType().IsPointer())
			{
				// Gives the value of the pointer to the reference
				// Since pointers and references are internally the same.
				llvm::Type* type = GenTypes::GetType(context, identifier->GetType()).LLVMType;
				return builder.CreateLoad(type, IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue, identifier->GetType().IsVolatile());
			}
			else // Reference -> Value
			{
				llvm::Type* ptrType = GenTypes::GetType(context, identifier->GetType()).LLVMType;
				llvm::Value* ptr = builder.CreateLoad(ptrType, IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue, identifier->GetType().IsVolatile());

				llvm::Type* valType = GenTypes::GetType(context, identifier->GetType().RemoveReference()).LLVMType;
				return builder.CreateLoad(valType, ptr, identifier->GetType().RemoveReference().IsVolatile());
			}
		}
		// Other expressions

		DY_ASSERT(0, "TODO");
		return nullptr;
	}

}

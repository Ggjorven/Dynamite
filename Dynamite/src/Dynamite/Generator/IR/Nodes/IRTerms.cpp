#include "dypch.h"
#include "IRTerms.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GenTypes.hpp"

#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"

#include "Dynamite/Generator/IR/Collections/IRScopeCollection.hpp"
#include "Dynamite/Generator/IR/Collections/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Terms
	/////////////////////////////////////////////////////////////////
	llvm::Value* IRTerms::GenTerm(const Node::Ref<Node::TermExpr> term, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		struct TermVisitor
		{
			llvm::LLVMContext& Context;
			llvm::IRBuilder<>& Builder;
			llvm::Module& Module;

			Optional<Type> EnforceType;

			llvm::Value* operator () (const Node::Ref<Node::LiteralTerm> obj) const
			{
				return GenLiteral(obj, Context, Builder, Module, EnforceType);
			}
			llvm::Value* operator () (const Node::Ref<Node::IdentifierTerm> obj) const
			{
				return GenIdentifier(obj, Context, Builder, Module, EnforceType);
			}
			llvm::Value* operator () (const Node::Ref<Node::ParenthesisTerm> obj) const
			{
				DY_ASSERT(0, "TODO");
				return nullptr;
			}
		};

		return std::visit(TermVisitor(context, builder, mod, enforceType), term->Term);
	}

	llvm::Value* IRTerms::GenLiteral(const Node::Ref<Node::LiteralTerm> lit, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		if (enforceType.HasValue())
			return GenTypes::GetValue(context, enforceType.Value(), lit->Literal).LLVMValue;
		
		return GenTypes::GetValue(context, lit->GetType(), lit->Literal).LLVMValue;
	}

	llvm::Value* IRTerms::GenIdentifier(const Node::Ref<Node::IdentifierTerm> identifier, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod, Optional<Type> enforceType)
	{
		llvm::Type* type = GenTypes::GetType(context, identifier->GetType()).LLVMType;

		if (enforceType.HasValue() && (identifier->GetType() != enforceType.Value()))
		{
			llvm::Value* nonCastValue = builder.CreateLoad(type, IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue);
			
			return GenTypes::Cast(builder, nonCastValue, identifier->GetType(), enforceType.Value());
		}

		return builder.CreateLoad(type, IRScopeCollection::GetVariable(identifier->Identifier).Value.LLVMValue);
	}

}

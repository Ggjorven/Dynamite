#include "dypch.h"
#include "IRFunctions.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

#include "Dynamite/Generator/IR/IRState.hpp"
#include "Dynamite/Generator/IR/Nodes/IRStatements.hpp"
#include "Dynamite/Generator/IR/Nodes/IRExpressions.hpp"

#include "Dynamite/Generator/IR/IRScopeCollection.hpp"
#include "Dynamite/Generator/IR/IRFunctionCollection.hpp"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Functions
	/////////////////////////////////////////////////////////////////
	void IRFunctions::GenDeclaration(const Node::Reference<Node::FunctionDeclaration> declaration, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		IRFunctionCollection::Function func = {};

		// Create function
		{
			llvm::Type* returnType = GeneratorTypes::GetType(context, declaration->GetType()).LLVMType;

			std::vector<llvm::Type*> parameters;
			parameters.reserve(declaration->Parameters.size());

			for (const auto& parameter : declaration->Parameters)
				parameters.push_back(GeneratorTypes::GetType(context, parameter->GetType()).LLVMType);

			llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, returnType, false);

			func.Callee = mod.getOrInsertFunction(declaration->Name.Value, funcType);
		}

		// Assign names to the function arguments
		{
			llvm::Function* llvmFunc = llvm::cast<llvm::Function>(func.Callee.getCallee());

			size_t argIndex = 0;
			for (auto arg = llvmFunc->arg_begin(); arg < llvmFunc->arg_end(); arg++)
			{
				Node::Reference<Node::VariableStatement> variable = declaration->Parameters[argIndex];

				llvm::Value* argValue = arg;
				argValue->setName(variable->Variable.Value);

				func.Arguments.emplace_back(argValue, variable->VariableType);

				argIndex++;
			}
		}

		IRFunctionCollection::AddFunction(declaration->Name.Value, func);
	}

	void IRFunctions::GenDefinition(const Node::Reference<Node::FunctionDefinition> definition, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		IRFunctionCollection::Function func = {};
		
		// Create function
		{
			llvm::Type* returnType = GeneratorTypes::GetType(context, definition->GetType()).LLVMType;

			std::vector<llvm::Type*> parameters;
			parameters.reserve(definition->Parameters.size());

			for (const auto& parameter : definition->Parameters)
				parameters.push_back(GeneratorTypes::GetType(context, parameter->GetType()).LLVMType);

			llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, returnType, false);

			func.Callee = mod.getOrInsertFunction(definition->Name.Value, funcType);
		}

		// Assign names to the function arguments
		{
			llvm::Function* llvmFunc = llvm::cast<llvm::Function>(func.Callee.getCallee());

			size_t argIndex = 0;
			for (auto arg = llvmFunc->arg_begin(); arg < llvmFunc->arg_end(); arg++)
			{
				Node::Reference<Node::VariableStatement> variable = definition->Parameters[argIndex];
				
				llvm::Value* argValue = arg;
				argValue->setName(variable->Variable.Value);

				func.Arguments.emplace_back(argValue, variable->VariableType);

				argIndex++;
			}
		}

		IRFunctionCollection::AddFunction(definition->Name.Value, func);
		
		// Generate body
		{
			// Set parsing state
			IRState::CurrentFunction = definition;

			llvm::Function* llvmFunc = llvm::cast<llvm::Function>(func.Callee.getCallee());

			llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", llvmFunc);
			builder.SetInsertPoint(entry);

			{
				IRScopeCollection::BeginScope();

				// Push parameters to the scope, so they are available
				for (size_t argIndex = 0; argIndex < func.Arguments.size(); argIndex++)
					IRScopeCollection::PushVar(definition->Parameters[argIndex]->Variable.Value, definition->Parameters[argIndex]->GetType(), func.Arguments[argIndex].first);

				IRStatements::GenScope(definition->Body, context, builder, mod);
			}

			IRState::CurrentFunction = nullptr;
		}
	}

	void IRFunctions::GenFunctionCall(const Node::Reference<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		auto& func = IRFunctionCollection::GetFunction(funcCall->Function.Value, funcCall->OverloadIndex);

		std::vector<llvm::Value*> arguments;
		arguments.reserve(func.Arguments.size());

		for (size_t argIndex = 0; argIndex < func.Arguments.size(); argIndex++)
		{
			auto argExpr = funcCall->Arguments[argIndex];
			llvm::Value* value = IRExpressions::GenExpression(argExpr, context, builder, mod, func.Arguments[argIndex].second);

			arguments.emplace_back(value);
		}

		builder.CreateCall(func.Callee, arguments);
	}

}

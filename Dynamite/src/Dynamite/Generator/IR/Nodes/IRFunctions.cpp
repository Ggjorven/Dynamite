#include "dypch.h"
#include "IRFunctions.hpp"

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
	// Functions
	/////////////////////////////////////////////////////////////////
	void IRFunctions::GenDeclaration(const Node::Ref<Node::FunctionDeclaration> declaration, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		GeneratorFunction func = {};

		// Create function
		{
			llvm::Type* returnType = GenTypes::GetType(context, declaration->GetType()).LLVMType;

			std::vector<llvm::Type*> parameters;
			parameters.reserve(declaration->Parameters.size());

			for (const auto& parameter : declaration->Parameters)
				parameters.push_back(GenTypes::GetType(context, parameter->GetType()).LLVMType);

			llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, parameters, declaration->VardiadicArguments);

			func.Callee = mod.getOrInsertFunction(declaration->Name, funcType);
			func.HasVardiadicArguments = declaration->VardiadicArguments;
		}

		// Assign names to the function arguments
		{
			llvm::Function* llvmFunc = llvm::cast<llvm::Function>(func.Callee.getCallee());

			size_t argIndex = 0;
			for (auto arg = llvmFunc->arg_begin(); arg < llvmFunc->arg_end(); arg++)
			{
				Node::Ref<Node::VariableStatement> variable = declaration->Parameters[argIndex];

				llvm::Value* argValue = arg;
				argValue->setName(variable->Variable);

				func.Arguments.emplace_back(variable->Variable, variable->VariableType, GeneratorValue(argValue));

				argIndex++;
			}
		}

		IRFunctionCollection::AddFunction(declaration->Name, func);
	}

	void IRFunctions::GenDefinition(const Node::Ref<Node::FunctionDefinition> definition, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		GeneratorFunction func = {};
		
		// Create function
		{
			llvm::Type* returnType = GenTypes::GetType(context, definition->GetType()).LLVMType;

			std::vector<llvm::Type*> parameters;
			parameters.reserve(definition->Parameters.size());

			for (const auto& parameter : definition->Parameters)
				parameters.push_back(GenTypes::GetType(context, parameter->GetType()).LLVMType);

			llvm::FunctionType* funcType = llvm::FunctionType::get(returnType, parameters, definition->VardiadicArguments);

			func.Callee = mod.getOrInsertFunction(definition->Name, funcType);
			func.HasVardiadicArguments = definition->VardiadicArguments;
		}

		// Assign names to the function arguments
		{
			llvm::Function* llvmFunc = llvm::cast<llvm::Function>(func.Callee.getCallee());

			size_t argIndex = 0;
			for (auto arg = llvmFunc->arg_begin(); arg < llvmFunc->arg_end(); arg++)
			{
				Node::Ref<Node::VariableStatement> variable = definition->Parameters[argIndex];
				
				llvm::Value* argValue = arg;
				argValue->setName(variable->Variable);

				func.Arguments.emplace_back(variable->Variable, variable->VariableType, GeneratorValue(argValue));

				argIndex++;
			}
		}

		IRFunctionCollection::AddFunction(definition->Name, func);
		
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
				{
					IRStatements::GenVariable(definition->Parameters[argIndex], context, builder, mod, func.Arguments[argIndex].Value);
				}

				IRStatements::GenScope(definition->Body, context, builder, mod, false, true);
			}

			// Reset parsing state
			IRState::CurrentFunction = nullptr;
		}
	}

	llvm::Value* IRFunctions::GenFunctionCall(const Node::Ref<Node::FunctionCall> funcCall, llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& mod)
	{
		auto& func = IRFunctionCollection::GetFunction(funcCall->Function, funcCall->OverloadIndex);

		std::vector<llvm::Value*> arguments;
		arguments.reserve(func.Arguments.size());

		for (size_t argIndex = 0; argIndex < funcCall->Arguments.size(); argIndex++)
		{
			auto argExpr = funcCall->Arguments[argIndex];
			if (argIndex < func.Arguments.size()) // Signature args
			{
				llvm::Value* value = IRExpressions::GenExpression(argExpr, context, builder, mod, func.Arguments[argIndex].VariableType);
				arguments.emplace_back(value);
			}
			else // Vardiadic args
			{
				llvm::Value* value = IRExpressions::GenExpression(argExpr, context, builder, mod);
				arguments.emplace_back(value);
			}
		}

		return builder.CreateCall(func.Callee, arguments);
	}

}

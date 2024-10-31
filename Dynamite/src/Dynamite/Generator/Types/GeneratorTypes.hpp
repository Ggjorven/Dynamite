#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// TypeRetrieval
	/////////////////////////////////////////////////////////////////
	struct TypeMetaData
	{
	public:
		llvm::Type* LLVMType = nullptr;
		Type OriginalType = {};
	};

	struct ValueMetaData
	{
	public:
		llvm::Value* LLVMValue = nullptr;
	};

	class GeneratorTypes
	{
	public:
		static TypeMetaData GetType(llvm::LLVMContext& context, const Type& originalType);

		static ValueMetaData GetValue(llvm::LLVMContext& context, const Type& type, const std::string& value);

		static llvm::Value* Cast(llvm::IRBuilder<>& builder, llvm::Value* value, llvm::Type* to);
	};

}
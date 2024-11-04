#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	struct GeneratorType
	{
	public:
		llvm::Type* LLVMType = nullptr;
		Type OriginalType = {};
	};

	struct GeneratorValue
	{
	public:
		llvm::Value* LLVMValue = nullptr;
	};

	/////////////////////////////////////////////////////////////////
	// Generator Types
	/////////////////////////////////////////////////////////////////
	class GenTypes
	{
	public:
		// Get functions
		static GeneratorType GetType(llvm::LLVMContext& context, const Type& originalType);
		static GeneratorValue GetValue(llvm::LLVMContext& context, const Type& type, const std::string& value);

	public:
		// Casting
		static llvm::Value* Cast(llvm::IRBuilder<>& builder, llvm::Value* value, const Type& from, const Type& to, const std::string& name = "cast");
	};

}
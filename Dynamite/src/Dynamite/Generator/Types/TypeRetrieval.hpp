#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include <llvm/IR/Type.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// TypeRetrieval
	/////////////////////////////////////////////////////////////////
	class GeneratorTypes
	{
	public:
		static llvm::Type* Get(llvm::LLVMContext& context, const Type& originalType);
	};

}
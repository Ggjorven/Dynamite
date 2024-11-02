#include "dypch.h"
#include "GenTypes.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

#include <llvm/IR/Constants.h>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Get functions
	/////////////////////////////////////////////////////////////////
	GeneratorType GenTypes::GetType(llvm::LLVMContext& context, const Type& originalType)
	{
		// Note: FrontQualifiers like mut & volatile are not used on LLVM's type level.

		GeneratorType returnType = {};
		returnType.OriginalType = originalType;

		switch (originalType.Information.Specifier)
		{
		case TypeSpecifier::Void:
			returnType.LLVMType = llvm::Type::getVoidTy(context);
			break;

		case TypeSpecifier::Bool:
			returnType.LLVMType = llvm::Type::getInt1Ty(context);
			break;

		case TypeSpecifier::Int8:
			returnType.LLVMType = llvm::Type::getInt8Ty(context);
			break;
		case TypeSpecifier::Int16:
			returnType.LLVMType = llvm::Type::getInt16Ty(context);
			break;
		case TypeSpecifier::Int32:
			returnType.LLVMType = llvm::Type::getInt32Ty(context);
			break;
		case TypeSpecifier::Int64:
			returnType.LLVMType = llvm::Type::getInt64Ty(context);
			break;

		case TypeSpecifier::UInt8:
			returnType.LLVMType = llvm::Type::getInt8Ty(context);
			break;
		case TypeSpecifier::UInt16:
			returnType.LLVMType = llvm::Type::getInt16Ty(context);
			break;
		case TypeSpecifier::UInt32:
			returnType.LLVMType = llvm::Type::getInt32Ty(context);
			break;
		case TypeSpecifier::UInt64:
			returnType.LLVMType = llvm::Type::getInt64Ty(context);
			break;

		case TypeSpecifier::Float32:
			returnType.LLVMType = llvm::Type::getFloatTy(context);
			break;
		case TypeSpecifier::Float64:
			returnType.LLVMType = llvm::Type::getDoubleTy(context);
			break;

		case TypeSpecifier::Char:
			returnType.LLVMType = llvm::Type::getInt8Ty(context);
			break;

		// Note: For the future
		case TypeSpecifier::Identifier:
			DY_ASSERT(0, "Identifier type handling not implemented.");
			break;

		default:
			DY_ASSERT(0, "Unknown type specifier.");
			break;
		}

		for (const auto& qualifier : originalType.BackQualifiers)
		{
			if (qualifier == TypeQualifier::Pointer)
			{
				returnType.LLVMType = llvm::PointerType::get(returnType.LLVMType, 0);
			}
			else if (qualifier == TypeQualifier::Array)
			{
				uint64_t arraySize = std::stoull(originalType.GetArraySize());
				returnType.LLVMType = llvm::ArrayType::get(returnType.LLVMType, arraySize);
			}
		}

		return returnType;
	}

	GeneratorValue GenTypes::GetValue(llvm::LLVMContext& context, const Type& type, const std::string& value)
	{
		GeneratorValue returnValue = {};

		switch (type.Information.Specifier)
		{
		case TypeSpecifier::Bool:
			// Convert string to bool
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), value == "1" ? 1 : 0);
			break;

		case TypeSpecifier::Int8:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), std::stoi(value));
			break;

		case TypeSpecifier::Int16:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), std::stoi(value));
			break;

		case TypeSpecifier::Int32:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), std::stoi(value));
			break;

		case TypeSpecifier::Int64:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), std::stoll(value));
			break;

		case TypeSpecifier::UInt8:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<uint8_t>(std::stoi(value)));
			break;

		case TypeSpecifier::UInt16:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), static_cast<uint16_t>(std::stoi(value)));
			break;

		case TypeSpecifier::UInt32:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), static_cast<uint32_t>(std::stoul(value)));
			break;

		case TypeSpecifier::UInt64:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), std::stoull(value));
			break;

		case TypeSpecifier::Float32:
			returnValue.LLVMValue = llvm::ConstantFP::get(llvm::Type::getFloatTy(context), std::stof(value));
			break;

		case TypeSpecifier::Float64:
			returnValue.LLVMValue = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), std::stod(value));
			break;

		case TypeSpecifier::Char:
			returnValue.LLVMValue = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<uint8_t>(value[0]));
			break;

		// Note: Handle other types as necessary
		default:
			DY_ASSERT(0, "Unknown type specifier for value conversion.");
			break;
		}

		return returnValue;
	}

	/////////////////////////////////////////////////////////////////
	// Casting
	/////////////////////////////////////////////////////////////////
	llvm::Value* GenTypes::Cast(llvm::IRBuilder<>& builder, llvm::Value* value, const Type& from, const Type& to)
	{
		llvm::Type* fromLLVM = GenTypes::GetType(builder.getContext(), from).LLVMType;
		llvm::Type* toLLVM = GenTypes::GetType(builder.getContext(), to).LLVMType;

		// Integers
		if (from.IsIntegral() && to.IsIntegral())
		{
			// Convert to smaller integer
			if (TypeCollection::GetLargest(from, to) == from)
			{
				return builder.CreateTrunc(value, toLLVM);
			}
			// Convert to larger integer
			{
				if (from.IsUnsigned())
					return builder.CreateZExt(value, toLLVM);
				else
					return builder.CreateSExt(value, toLLVM);
			}
		}

		// Floats
		else if (from.IsFloat() && to.IsFloat())
		{
			// Convert to smaller float
			if (TypeCollection::GetLargest(from, to) == from)
			{
				return builder.CreateFPTrunc(value, toLLVM);
			}
			else
			{
				return builder.CreateFPExt(value, toLLVM);
			}
		}

		// Int -> float
		else if (from.IsIntegral() && to.IsFloat())
		{
			if (from.IsUnsigned())
				return builder.CreateUIToFP(value, toLLVM);
			else
				return builder.CreateSIToFP(value, toLLVM);
		}

		// Float -> int
		else if (from.IsFloat() && to.IsIntegral())
		{
			if (to.IsUnsigned())
				return builder.CreateFPToUI(value, toLLVM);
			else
				return builder.CreateFPToSI(value, toLLVM);
		}

		// Pointers
		else if (from.IsPointer() && to.IsPointer())
		{
			return builder.CreateBitCast(value, toLLVM);
		}

		DY_ASSERT(0, "[Internal Compiler Error] - Casting for these types has not been implemented.");
		return nullptr;
	}


}
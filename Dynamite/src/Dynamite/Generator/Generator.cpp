#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

// No ASM
#include "Dynamite/Generator/LLVM/LLVMGenerator.hpp"

namespace Dynamite
{

	Pulse::Unique<Generator> Generator::Create(Type type)
	{
		switch (type)
		{
		case Type::ASM:
			//return Pulse::Unique<ASMGenerator>::Create();
		case Type::LLVM:
			return Pulse::Unique<LLVMGenerator>::Create();

		default:
			break;
		}

		return nullptr;
	}

}
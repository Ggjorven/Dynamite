#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Generator/ASMGenerator.hpp"

#include <Pulse/Text/Format.hpp>

namespace Dynamite
{

	Pulse::Unique<Generator> Generator::Create(Type type)
	{
		switch (type)
		{
		case Type::ASM:
			return Pulse::Unique<ASMGenerator>::Create();
		case Type::LLVM:
			//return Unique<LLVMGenerator>::Create();

		default:
			break;
		}

		return nullptr;
	}

}
#include "dypch.h"
#include "OperationType.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Helper methods
	/////////////////////////////////////////////////////////////////
	size_t GetOperationPrecedence(OperationType operation)
	{
		switch (operation)
		{
		case OperationType::Add:
		case OperationType::Subtract:
			return 0;

		case OperationType::Multiply:
		case OperationType::Divide:
			return 1;

		case OperationType::AND:
			return 2;

		case OperationType::XOR:
			return 3;

		case OperationType::OR:
			return 4;

		case OperationType::Reference:
		case OperationType::Address:
		case OperationType::Dereference:
			return 5;

		default:
			break;
		}

		return Pulse::Numeric::Max<size_t>();
	}

}
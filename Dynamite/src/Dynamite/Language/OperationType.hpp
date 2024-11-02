#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Operation
	/////////////////////////////////////////////////////////////////
	enum class OperationType : uint8_t
	{
		None = 0,

		Add,
		Subtract,
		Divide,
		Multiply,

		OR,
		AND,
		XOR,

		Reference,
		Address,
		Dereference,
	};

	/////////////////////////////////////////////////////////////////
	// Helper methods
	/////////////////////////////////////////////////////////////////
	size_t GetOperationPrecedence(OperationType operation);

}
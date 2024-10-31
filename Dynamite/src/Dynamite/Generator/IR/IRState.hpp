#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

namespace Dynamite
{

	class IRState
	{
	public:
		inline static Node::Reference<Node::FunctionDefinition> CurrentFunction = (Node::Reference<Node::FunctionDefinition>)Node::NullRef;
	};

}
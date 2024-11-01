#pragma once

#include "Core/Core.hpp"

#include "Language/Nodes/Program.hpp"

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// IRState
	/////////////////////////////////////////////////////////////////
	class IRState
	{
	public:
		inline static Node::Ref<Node::FunctionDefinition> CurrentFunction = (Node::Ref<Node::FunctionDefinition>)Node::NullRef;
	};

}
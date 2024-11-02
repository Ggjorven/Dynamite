#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

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
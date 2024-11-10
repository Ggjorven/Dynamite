#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// ParserState
	/////////////////////////////////////////////////////////////////
	class ParserState
	{
	public:
		Language::Node::Ref<Language::Node::FunctionDefinition> CurrentFunction = (Language::Node::Ref<Language::Node::FunctionDefinition>)Language::Node::NullRef;
	};

}
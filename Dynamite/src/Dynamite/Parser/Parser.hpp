#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

namespace Dynamite
{

	class Parser
	{
	public:
		Parser(std::vector<Token>& tokens);
		~Parser() = default;
	};

}
#include "dypch.h"
#include "Dynamite/Dynamite/Parser/Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

#include "Dynamite/Language/Types/TypeCollection.hpp"

#include <Pulse/Core/Defines.hpp>

namespace Dynamite
{

	using namespace Language;

	/////////////////////////////////////////////////////////////////
	// Function parsing
	/////////////////////////////////////////////////////////////////
	Optional<Node::Ref<Node::FunctionCall>> Parser::ParseFunctionCall()
	{
		// TODO: ...
		return {};
	}



	Optional<Node::Ref<Node::Function>> Parser::ParseFunction()
	{
		// TODO: ...
		return {};
	}

}
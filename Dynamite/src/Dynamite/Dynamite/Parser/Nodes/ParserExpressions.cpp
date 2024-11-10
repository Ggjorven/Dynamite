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
	// Expression parsing
	/////////////////////////////////////////////////////////////////
    Optional<Node::Ref<Node::TermExpr>> Parser::ParseTermExpr()
	{
		// TODO: ...
		return {};
	}



	void Parser::HandleBinaryOperators(Node::Ref<Node::Expression>& expr, size_t minimumPrecedence)
	{
		// TODO: ...
	}



	Optional<Node::Ref<Node::Expression>> Parser::ParseExpression(size_t minimumPrecedence)
	{
		// TODO: ...
		return {};
	}
    
}
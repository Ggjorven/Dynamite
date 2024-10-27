#include "dypch.h"
#include "Parser.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

#include "Dynamite/Parser/Systems/TypeSystem.hpp"
#include "Dynamite/Parser/Systems/ScopeSystem.hpp"
#include "Dynamite/Parser/Systems/FunctionSystem.hpp"

#include <Pulse/Core/Defines.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Condition parsing
	/////////////////////////////////////////////////////////////////
    Optional<Node::Reference<Node::ConditionBranch>> Parser::ParseConditionBrach()
	{
		/////////////////////////////////////////////////////////////////
		// Else If Branch
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Else) 
			&& Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::If))
		{
			Consume(); // 'else' token
			Consume(); // 'if' token

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				auto elif = Node::New<Node::ElseIfBranch>(expr.Value());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

				if (auto scope = ParseScopeStatement())
				{
					elif->Scope = scope.Value();
					elif->Next = ParseConditionBrach(); // Note: Can be NullRef

					return Node::New<Node::ConditionBranch>(elif);
				}
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			return {};
		}

		/////////////////////////////////////////////////////////////////
		// Else Branch
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Else))
		{
			Consume(); // 'else' token

			if (auto scope = ParseScopeStatement())
			{
				return Node::New<Node::ConditionBranch>(Node::New<Node::ElseBranch>(scope.Value()));
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");

			return {};
		}

		return {};
	}

}
#include "dypch.h"
#include "Dynamite/Parser/Parser.hpp"

#include "Core/Logging.hpp"

#include "Compiler/Compiler.hpp"

#include "Utils/Utils.hpp"
#include "Utils/Checks.hpp"
#include "Utils/Optional.hpp"

#include "Language/Types/TypeCollection.hpp"

#include <Pulse/Core/Defines.hpp>

namespace Dynamite
{

	using namespace Language;

	/////////////////////////////////////////////////////////////////
	// Condition parsing
	/////////////////////////////////////////////////////////////////
    Optional<Node::Ref<Node::ConditionBranch>> Parser::ParseConditionBrach()
	{
		/////////////////////////////////////////////////////////////////
		// Else If Branch
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Else) 
			&& Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::If))
		{
			Node::Ref<Node::ConditionBranch> condition = m_Tracker.New<Node::ConditionBranch>();
			
			Consume(); // 'else' token
			Consume(); // 'if' token

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

				auto elif = Node::New<Node::ElseIfBranch>(expr.Value());
				condition->Branch = elif;

				if (auto scope = ParseScopeStatement())
				{
					elif->Scope = scope.Value();
					elif->Next = ParseConditionBrach(); // Note: Can be NullRef

					return m_Tracker.Return<Node::ConditionBranch>();
				}
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			m_Tracker.Pop<Node::ConditionBranch>();
			return {};
		}

		/////////////////////////////////////////////////////////////////
		// Else Branch
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Else))
		{
			Node::Ref<Node::ConditionBranch> condition = m_Tracker.New<Node::ConditionBranch>();

			Consume(); // 'else' token

			if (auto scope = ParseScopeStatement())
			{
				condition->Branch = Node::New<Node::ElseBranch>(scope.Value());

				return m_Tracker.Return<Node::ConditionBranch>();
			}
			
			Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			m_Tracker.Pop<Node::ConditionBranch>();
			return {};
		}

		return {};
	}

}
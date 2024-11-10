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
				if (!TypeCollection::ImplicitCastable(expr.Value()->GetType(), Type(TypeSpecifier::Bool)))
					Compiler::Error(Peek(-1).Value().LineNumber, "Condition branch's expression must be a boolean type.");

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
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
	// Expression parsing
	/////////////////////////////////////////////////////////////////
    Optional<Node::Reference<Node::TermExpr>> Parser::ParseTermExpr()
	{
		/////////////////////////////////////////////////////////////////
		// Literal
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeLiterals()))
		{
			auto consumed = Consume();

			Node::Reference<Node::TermExpr> termExpr = Node::New<Node::TermExpr>();

			Type literalType = TypeSystem::GetTypeFromLiteral(consumed.Type, consumed.Value);
			Token literalToken = consumed;
			
			termExpr->Term = Node::New<Node::LiteralTerm>(literalType, literalToken);

			return termExpr;
		}

		/////////////////////////////////////////////////////////////////
		// FunctionCall
		/////////////////////////////////////////////////////////////////
		else if (auto funcCall = ParseFunctionCall())
		{
			return Node::New<Node::TermExpr>(funcCall.Value());
		}

		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier))
		{
			auto consumed = Consume();

			Node::Reference<Node::TermExpr> termExpr = Node::New<Node::TermExpr>();

			Optional<Type> identifierType = ScopeSystem::GetVariableType(consumed.Value);
			if (!identifierType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Undeclared identifier: {0}", consumed.Value);
				return {};
			}

			Token identifierToken = consumed;

			termExpr->Term = Node::New<Node::IdentifierTerm>(identifierType.Value(), identifierToken);

			return termExpr;
		}

		/////////////////////////////////////////////////////////////////
		// Parenthesis
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenParenthesis))
		{
			Consume(); // '(' token

			auto expr = ParseExpression();
			if (!expr.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve a valid expression");
				return {};
			}

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`");

			return Node::New<Node::TermExpr>(Node::New<Node::ParenthesisTerm>(expr.Value()));
		}

		return {};
	}



	Optional<Node::Reference<Node::Expression>> Parser::ParseExpression(size_t minimumPrecedence)
	{
		/////////////////////////////////////////////////////////////////
		// TermExpr
		/////////////////////////////////////////////////////////////////
		if (auto term = ParseTermExpr())
		{
			/////////////////////////////////////////////////////////////////
			// Expression retrieval/creation
			/////////////////////////////////////////////////////////////////
			Node::Reference<Node::Expression> expr = Node::New<Node::Expression>(term.Value());

			while (true)
			{
				Optional<Token> current = Peek(0);
				Optional<size_t> precedence = {};
				size_t nextMinimumPrecedence = Pulse::Numeric::Max<size_t>();

				// Note: It breaks out and just returns the normal expression if
				// it's not a binary expression.
				if (!current.HasValue())
					break;

				precedence = Node::GetBinaryPrecendence(current.Value().Type);
				if (!precedence.HasValue() || precedence.Value() < minimumPrecedence)
					break;
				else
					nextMinimumPrecedence = precedence.Value() + 1;

				auto exprLHS = Node::New<Node::Expression>(expr->Expr);
				Token operation = Consume();

				auto exprRHS = ParseExpression(nextMinimumPrecedence);
				if (!exprRHS.HasValue())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Unable to parse expression.");
					break;
				}

				auto binExpr = Node::New<Node::BinaryExpr>(operation.Type, TypeSystem::GetResultType(exprLHS->GetType(), exprRHS.Value()->GetType()), exprLHS, exprRHS.Value());

				// Set the result expr to the computed binary expression
				expr->Expr = binExpr;
			}

			// Note: This is either a binary expression or just a normal expression.
			// The loop up top accounts for both
			return expr;
		}

		return {};
	}
    
}
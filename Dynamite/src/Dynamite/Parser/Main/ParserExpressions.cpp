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
			Node::Reference<Node::TermExpr> termExpr = m_Tracker.New<Node::TermExpr>();

			auto consumed = Consume();
			
			auto literalTerm = Node::New<Node::LiteralTerm>();
			literalTerm->LiteralType = TypeSystem::GetTypeFromLiteral(consumed.Type, consumed.Value);
			literalTerm->Literal = consumed;

			termExpr->Term = literalTerm;

			return m_Tracker.Return<Node::TermExpr>();
		}

		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier))
		{
			Node::Reference<Node::TermExpr> termExpr = m_Tracker.New<Node::TermExpr>();

			auto consumed = Consume();

			Optional<Type> identifierType = ScopeSystem::GetVariableType(consumed.Value);
			if (!identifierType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Undeclared identifier: {0}", consumed.Value);
				m_Tracker.Pop<Node::TermExpr>();
				return {};
			}

			auto identifierTerm = Node::New<Node::IdentifierTerm>();
			identifierTerm->IdentifierType = identifierType.Value();
			identifierTerm->Identifier = consumed;

			termExpr->Term = identifierTerm;

			return m_Tracker.Return<Node::TermExpr>();
		}

		/////////////////////////////////////////////////////////////////
		// Parenthesis
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenParenthesis))
		{
			Node::Reference<Node::TermExpr> termExpr = m_Tracker.New<Node::TermExpr>();

			Consume(); // '(' token

			auto expr = ParseExpression();
			if (!expr.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve a valid expression");
				m_Tracker.Pop<Node::TermExpr>();
				return {};
			}

			auto parenthesis = Node::New<Node::ParenthesisTerm>();
			parenthesis->Expr = expr.Value();

			termExpr->Term = parenthesis;

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`");
			return m_Tracker.Return<Node::TermExpr>();
		}

		return {};
	}



	void Parser::HandleBinaryOperators(Node::Reference<Node::Expression>& expr, size_t minimumPrecedence)
	{
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

			auto binExpr = Node::New<Node::BinaryExpr>(operation.Type, TypeSystem::GetBinExprResultType(exprLHS->GetType(), operation.Type, exprRHS.Value()->GetType()), exprLHS, exprRHS.Value());

			// Set the result expr to the computed binary expression
			expr->Expr = binExpr;
		}
	}



	Optional<Node::Reference<Node::Expression>> Parser::ParseExpression(size_t minimumPrecedence)
	{
		/////////////////////////////////////////////////////////////////
		// Function call
		/////////////////////////////////////////////////////////////////
		if (auto funcCall = ParseFunctionCall())
		{
			Node::Reference<Node::Expression> expr = m_Tracker.New<Node::Expression>(funcCall.Value());

			// Note: This is either a binary expression or just a normal expression.
			// The function accounts for both
			HandleBinaryOperators(expr, minimumPrecedence);

			return m_Tracker.Return<Node::Expression>();
		}

		/////////////////////////////////////////////////////////////////
		// Address
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::And))
		{
			Node::Reference<Node::Expression> expr = m_Tracker.New<Node::Expression>();
			Node::Reference<Node::AddressExpr> address = Node::New<Node::AddressExpr>();

			expr->Expr = address;

			Consume(); // '&' token

			if (auto addressExpr = ParseExpression(Node::GetMaxBinaryPrecendence()))
			{
				address->Expr = addressExpr.Value();

				if (!addressExpr.Value()->IsLValue())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Can't take memory address of an RValue.");
					m_Tracker.Pop<Node::Expression>();
					return {};
				}

				// Note: This is either a binary expression or just a normal expression.
				// The function accounts for both
				HandleBinaryOperators(expr, minimumPrecedence);

				return m_Tracker.Return<Node::Expression>();
			}

			Compiler::Error(Peek(-1).Value().LineNumber, "Expected expression.");
			return m_Tracker.Return<Node::Expression>();
		}

		/////////////////////////////////////////////////////////////////
		// Dereference
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Pointer))
		{
			Node::Reference<Node::Expression> expr = m_Tracker.New<Node::Expression>();
			Node::Reference<Node::DereferenceExpr> dereference = Node::New<Node::DereferenceExpr>();

			expr->Expr = dereference;

			Consume(); // '*' token

			if (auto dereferenceExpr = ParseExpression(Node::GetMaxBinaryPrecendence()))
			{
				dereference->Expr = dereferenceExpr.Value();
				
				if (!dereferenceExpr.Value()->IsLValue())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Can't dereference an RValue.");
					m_Tracker.Pop<Node::Expression>();
					return {};
				}
				else if (!dereferenceExpr.Value()->GetType().IsPointer())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Can't dereference an non-pointer.");
					m_Tracker.Pop<Node::Expression>();
					return {};
				}

				// Note: This is either a binary expression or just a normal expression.
				// The function accounts for both
				HandleBinaryOperators(expr, minimumPrecedence);

				return m_Tracker.Return<Node::Expression>();
			}

			Compiler::Error(Peek(-1).Value().LineNumber, "Expected expression.");
			return m_Tracker.Return<Node::Expression>();
		}

		/////////////////////////////////////////////////////////////////
		// Term
		/////////////////////////////////////////////////////////////////
		else if (auto term = ParseTermExpr())
		{
			Node::Reference<Node::Expression> expr = m_Tracker.New<Node::Expression>(term.Value());

			// Note: This is either a binary expression or just a normal expression.
			// The function accounts for both
			HandleBinaryOperators(expr, minimumPrecedence);

			return m_Tracker.Return<Node::Expression>();
		}

		return {};
	}
    
}
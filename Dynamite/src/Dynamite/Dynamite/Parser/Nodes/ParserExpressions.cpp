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
		/////////////////////////////////////////////////////////////////
		// Literal
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, GetAllTokenTypeLiterals()))
		{
			Node::Ref<Node::TermExpr> termExpr = m_Tracker.New<Node::TermExpr>();

			auto consumed = Consume();
			
			auto literalTerm = Node::New<Node::LiteralTerm>();
			literalTerm->LitType = TokenTypeToLiteralType(consumed.Type);
			literalTerm->Literal = consumed.Value;
			literalTerm->ValueType = TypeCollection::GetTypeFromLiteral(literalTerm->LitType, consumed.Value);

			termExpr->Term = literalTerm;

			return m_Tracker.Return<Node::TermExpr>();
		}

		/////////////////////////////////////////////////////////////////
		// Identifier
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier))
		{
			Node::Ref<Node::TermExpr> termExpr = m_Tracker.New<Node::TermExpr>();

			auto consumed = Consume();

			Optional<Type> identifierType = m_Scopes.GetVariableType(consumed.Value);
			if (!identifierType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Undeclared identifier: {0}", consumed.Value);
				m_Tracker.Pop<Node::TermExpr>();
				return {};
			}

			auto identifierTerm = Node::New<Node::IdentifierTerm>();
			identifierTerm->IdentifierType = identifierType.Value();
			identifierTerm->Identifier = consumed.Value;

			termExpr->Term = identifierTerm;

			return m_Tracker.Return<Node::TermExpr>();
		}

		/////////////////////////////////////////////////////////////////
		// Parenthesis
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenParenthesis))
		{
			Node::Ref<Node::TermExpr> termExpr = m_Tracker.New<Node::TermExpr>();

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



	void Parser::HandleBinaryOperators(Node::Ref<Node::Expression>& expr, size_t minimumPrecedence)
	{
		while (true)
		{
			Optional<Token> current = Peek(0);
			size_t precedence = Pulse::Numeric::Max<size_t>();
			size_t nextMinimumPrecedence = Pulse::Numeric::Max<size_t>();

			// Note: It breaks out and just returns the normal expression if
			// it's not a binary expression.
			if (!current.HasValue())
				break;

			// If it's not a binary expr break
			if (!Utils::OptMemberIs(current, &Token::Type, GetAllTokenTypeOperators()))
				break;

			precedence = GetOperationPrecedence(TokenTypeToOperationType(current.Value().Type));
			if (precedence < minimumPrecedence)
				break;
			else
				nextMinimumPrecedence = precedence + 1;

			auto exprLHS = Node::New<Node::Expression>(expr->Expr);
			Token operation = Consume();

			auto exprRHS = ParseExpression(nextMinimumPrecedence);
			if (!exprRHS.HasValue())
			{
				Compiler::Error(Peek(-1).Value().LineNumber, "Unable to parse expression.");
				break;
			}

			auto binExpr = Node::New<Node::BinaryExpr>(TokenTypeToOperationType(operation.Type), TypeCollection::GetBinExprResultType(exprLHS->GetType(), TokenTypeToOperationType(operation.Type), exprRHS.Value()->GetType()), exprLHS, exprRHS.Value());

			// Set the result expr to the computed binary expression
			expr->Expr = binExpr;
		}
	}



	Optional<Node::Ref<Node::Expression>> Parser::ParseExpression(size_t minimumPrecedence)
	{
		/////////////////////////////////////////////////////////////////
		// Function call
		/////////////////////////////////////////////////////////////////
		if (auto funcCall = ParseFunctionCall())
		{
			Node::Ref<Node::Expression> expr = m_Tracker.New<Node::Expression>(funcCall.Value());

			// Note: This is either a binary expression or just a normal expression.
			// The function accounts for both
			HandleBinaryOperators(expr, minimumPrecedence);

			return m_Tracker.Return<Node::Expression>();
		}

		/////////////////////////////////////////////////////////////////
		// Reference
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::TakeReference))
		{
			Node::Ref<Node::Expression> expr = m_Tracker.New<Node::Expression>();
			Node::Ref<Node::ReferenceExpr> reference = Node::New<Node::ReferenceExpr>();

			expr->Expr = reference;

			Consume(); // '&' token

			if (auto referenceExpr = ParseExpression(GetOperationPrecedence(OperationType::Reference)))
			{
				reference->Expr = referenceExpr.Value();

				if (!referenceExpr.Value()->IsLValue())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Can't take reference of an RValue.");
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
		// Address
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::TakeAddress))
		{
			Node::Ref<Node::Expression> expr = m_Tracker.New<Node::Expression>();
			Node::Ref<Node::AddressExpr> address = Node::New<Node::AddressExpr>();

			expr->Expr = address;

			Consume(); // '#' token

			if (auto addressExpr = ParseExpression(GetOperationPrecedence(OperationType::Address)))
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
			Node::Ref<Node::Expression> expr = m_Tracker.New<Node::Expression>();
			Node::Ref<Node::DereferenceExpr> dereference = Node::New<Node::DereferenceExpr>();

			expr->Expr = dereference;

			Consume(); // '*' token

			if (auto deRefExpr = ParseExpression(GetOperationPrecedence(OperationType::Dereference)))
			{
				dereference->Expr = deRefExpr.Value();
				
				if (!deRefExpr.Value()->IsLValue())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Can't deRef an RValue.");
					m_Tracker.Pop<Node::Expression>();
					return {};
				}
				else if (!deRefExpr.Value()->GetType().IsPointer())
				{
					Compiler::Error(Peek(-1).Value().LineNumber, "Can't deRef an non-pointer.");
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
			Node::Ref<Node::Expression> expr = m_Tracker.New<Node::Expression>(term.Value());

			// Note: This is either a binary expression or just a normal expression.
			// The function accounts for both
			HandleBinaryOperators(expr, minimumPrecedence);

			return m_Tracker.Return<Node::Expression>();
		}

		return {};
	}
    
}
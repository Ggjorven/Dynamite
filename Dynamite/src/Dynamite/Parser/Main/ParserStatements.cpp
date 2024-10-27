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
	// Statements parsing
	/////////////////////////////////////////////////////////////////
    Optional<Node::Reference<Node::IfStatement>> Parser::ParseIfStatement()
	{
		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::If))
		{
			auto consumed = Consume();

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				auto ifStatement = Node::New<Node::IfStatement>(expr.Value());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

				if (auto scope = ParseScopeStatement())
				{
					ifStatement->Scope = scope.Value();
					ifStatement->Next = ParseConditionBrach(); // Note: Can be NullRef

					return ifStatement;
				}
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			return {};
		}

		return {};
	}



	Optional<Node::Reference<Node::ScopeStatement>> Parser::ParseScopeStatement(bool startScope)
	{
		if (!Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
			return {};

		Consume(); // '{' token

		if (startScope)
			ScopeSystem::BeginScope();

		Node::Reference<Node::ScopeStatement> scope = Node::New<Node::ScopeStatement>();
		while (auto statement = ParseStatement())
			scope->Statements.push_back(statement.Value());

		ScopeSystem::EndScope();

		CheckConsume(TokenType::CloseCurlyBrace, "Expected `}}`");
		return scope;
	}



	Optional<Node::Reference<Node::VariableStatement>> Parser::ParseVariableStatement()
	{
		size_t offset = 0;
		
		/////////////////////////////////////////////////////////////////
		// Variable initiazation
		/////////////////////////////////////////////////////////////////
		if (PeekIsType(offset) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Equals))
		{
			Optional<Type> type = GetType();
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type for variable.");
				return {};
			}

			Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>(type.Value(), Consume()); // Identifier token

			std::string varName = variable->Variable.Value;
			ScopeSystem::PushVar(varName, type.Value());

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpression())
			{
				if (!TypeSystem::Castable(expr.Value()->GetType(), type.Value()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Variable creation of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", varName, TypeSystem::ToString(type.Value()), TypeSystem::ToString(expr.Value()->GetType()));

					// Semicolon `;` resolution
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return variable;
				}

				// Note: Only casts if the internal type is a literalterm
				Cast(expr.Value()->GetType(), type.Value(), expr.Value());
				variable->Expr = expr.Value();
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			// Semicolon ';' resolution
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return variable;
		}
		
		/////////////////////////////////////////////////////////////////
		// Variable Default Initialization
		/////////////////////////////////////////////////////////////////
		offset = 0;

		if (PeekIsType(offset) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Semicolon))
		{
			Optional<Type> type = GetType();
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type for variable.");
				return {};
			}

			Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>(type.Value(), Consume()); // Identifier token

			std::string varName = variable->Variable.Value;
			ScopeSystem::PushVar(varName, type.Value());

			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return variable;
		}

		return {};
	}



	Optional<Node::Reference<Node::Statement>> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Scope
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
		{
			if (auto scope = ParseScopeStatement())
				return Node::New<Node::Statement>(scope.Value());
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid scope.");
		}

		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		else if (auto if_ = ParseIfStatement())
		{
			return Node::New<Node::Statement>(if_.Value());
		}

		/////////////////////////////////////////////////////////////////
		// Variable creation
		/////////////////////////////////////////////////////////////////
		else if (auto var = ParseVariableStatement())
		{
			return Node::New<Node::Statement>(var.Value());
		}

		/////////////////////////////////////////////////////////////////
		// Function call
		/////////////////////////////////////////////////////////////////
		else if (auto funcCall = ParseFunctionCall())
		{
			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			return Node::New<Node::Statement>(funcCall.Value());
		}

		/////////////////////////////////////////////////////////////////
		// Return
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Return))
		{
			Consume(); // Return token

			if (auto expr = ParseExpression())
			{
				// TODO: Check if returnStatement matches the returnType of the function

				auto returnStatement = Node::New<Node::ReturnStatement>(expr.Value());
				CheckConsume(TokenType::Semicolon, "Expected `;`.");

				return Node::New<Node::Statement>(returnStatement);
			}

			Compiler::Error(Peek(-1).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return {};
		}

		/////////////////////////////////////////////////////////////////
		// Variable assignment
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier) && 
			Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::Equals))
		{
			auto consumed = Consume();
			auto type = ScopeSystem::GetVariableType(consumed.Value);
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Undeclared identifier: {0}", consumed.Value);
				return {};
			}

			auto assignment = Node::New<Node::AssignmentStatement>(type.Value(), consumed);
			Consume(); // '=' token

			if (auto expr = ParseExpression())
			{
				if (!TypeSystem::Castable(expr.Value()->GetType(), type.Value()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Variable assignment of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", consumed.Value, TypeSystem::ToString(type.Value()), TypeSystem::ToString(expr.Value()->GetType()));

					// Semicolon `;` resolution
					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return Node::New<Node::Statement>(assignment);
				}

				// Note: Only casts if the internal type is a literalterm
				Cast(expr.Value()->GetType(), type.Value(), expr.Value());
				assignment->Expr = expr.Value();

				CheckConsume(TokenType::Semicolon, "Expected `;`.");

				return Node::New<Node::Statement>(assignment);
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			return {};
		}

		return {};
	}

}
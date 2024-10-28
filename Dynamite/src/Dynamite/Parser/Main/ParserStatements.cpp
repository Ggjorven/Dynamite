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
			Node::Reference<Node::IfStatement> ifStatement = m_Tracker.New<Node::IfStatement>();

			Consume(); // If token

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				ifStatement->Expr = expr.Value();

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
				if (auto scope = ParseScopeStatement())
				{
					ifStatement->Scope = scope.Value();
					ifStatement->Next = ParseConditionBrach(); // Note: Can be NullRef

					return m_Tracker.Return<Node::IfStatement>();
				}
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Failed to retrieve valid scope.");
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			m_Tracker.Pop<Node::IfStatement>();
			return {};
		}

		return {};
	}



	Optional<Node::Reference<Node::ScopeStatement>> Parser::ParseScopeStatement(bool startScope)
	{
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
		{
			Node::Reference<Node::ScopeStatement> scope = m_Tracker.New<Node::ScopeStatement>();
			
			Consume(); // '{' token

			if (startScope)
				ScopeSystem::BeginScope();

			while (auto statement = ParseStatement())
				scope->Statements.push_back(statement.Value());

			ScopeSystem::EndScope();

			CheckConsume(TokenType::CloseCurlyBrace, "Expected `}}`");
			return m_Tracker.Return<Node::ScopeStatement>();
		}

		return {};
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
			Node::Reference<Node::VariableStatement> variable =m_Tracker.New<Node::VariableStatement>();

			Optional<Type> type = GetType();
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type for variable.");
				return {};
			}

			variable->VariableType = type.Value();
			variable->Variable = Consume(); // Identifier token

			ScopeSystem::PushVar(variable->Variable.Value, type.Value());

			Consume(); // '=' token

			// Expression resolution
			if (auto expr = ParseExpression())
			{
				if (!TypeSystem::Castable(expr.Value()->GetType(), type.Value()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Variable creation of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", variable->Variable.Value, TypeSystem::ToString(type.Value()), TypeSystem::ToString(expr.Value()->GetType()));

					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return m_Tracker.Return<Node::VariableStatement>();
				}

				// Note: Only casts if the internal type is a literalterm
				Cast(expr.Value()->GetType(), type.Value(), expr.Value());
				variable->Expr = expr.Value();
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");

			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			return m_Tracker.Return<Node::VariableStatement>();
		}
		
		/////////////////////////////////////////////////////////////////
		// Variable Default Initialization
		/////////////////////////////////////////////////////////////////
		offset = 0;

		if (PeekIsType(offset) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Semicolon))
		{
			Node::Reference<Node::VariableStatement> variable = m_Tracker.New<Node::VariableStatement>();

			Optional<Type> type = GetType();
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type for variable.");
				return {};
			}

			variable->VariableType = type.Value();
			variable->Variable = Consume(); // Identifier token

			ScopeSystem::PushVar(variable->Variable.Value, type.Value());

			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			return m_Tracker.Return<Node::VariableStatement>();
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
			if (auto scopeStatement = ParseScopeStatement())
			{
				Node::Reference<Node::Statement> statement = m_Tracker.New<Node::Statement>();
				statement->StatementObj = scopeStatement.Value();

				return m_Tracker.Return<Node::Statement>();
			}
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid scope.");
		}

		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		else if (auto ifStatement = ParseIfStatement())
		{
			Node::Reference<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			statement->StatementObj = ifStatement.Value();

			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Variable creation
		/////////////////////////////////////////////////////////////////
		else if (auto varStatement = ParseVariableStatement())
		{
			Node::Reference<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			statement->StatementObj = varStatement.Value();

			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Function call
		/////////////////////////////////////////////////////////////////
		else if (auto funcCall = ParseFunctionCall())
		{
			Node::Reference<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			statement->StatementObj = funcCall.Value();

			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Return
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Return))
		{
			Node::Reference<Node::Statement> statement = m_Tracker.New<Node::Statement>();

			Consume(); // Return token

			if (auto expr = ParseExpression())
			{
				auto returnStatement = Node::New<Node::ReturnStatement>(expr.Value());
				
				// TODO: Check if returnStatement matches the returnType of the function
				
				statement->StatementObj = returnStatement;

				CheckConsume(TokenType::Semicolon, "Expected `;`.");
				return m_Tracker.Return<Node::Statement>();
			}

			Compiler::Error(Peek(-1).Value().LineNumber, "Invalid expression.");
			
			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			m_Tracker.Pop<Node::Statement>();
			return {};
		}

		/////////////////////////////////////////////////////////////////
		// Variable assignment
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier) && 
			Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::Equals))
		{
			Node::Reference<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			auto assignment = Node::New<Node::AssignmentStatement>();

			statement->StatementObj = assignment;

			assignment->Variable = Consume(); // Identifier token

			Optional<Type> type = ScopeSystem::GetVariableType(assignment->Variable.Value);
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Undeclared identifier: {0}", assignment->Variable.Value);
				m_Tracker.Pop<Node::Statement>();
				return {};
			}

			Consume(); // '=' token

			if (auto expr = ParseExpression())
			{
				if (!TypeSystem::Castable(expr.Value()->GetType(), type.Value()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Variable assignment of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", assignment->Variable.Value, TypeSystem::ToString(type.Value()), TypeSystem::ToString(expr.Value()->GetType()));

					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return m_Tracker.Return<Node::Statement>();
				}

				// Note: Only casts if the internal type is a literalterm
				Cast(expr.Value()->GetType(), type.Value(), expr.Value());
				assignment->Expr = expr.Value();

				CheckConsume(TokenType::Semicolon, "Expected `;`.");
				return m_Tracker.Return<Node::Statement>();
			}

			Compiler::Error(Peek(0).Value().LineNumber, "Invalid expression.");
			m_Tracker.Pop<Node::Statement>();
			return {};
		}

		return {};
	}

}
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
	// Statements parsing
	/////////////////////////////////////////////////////////////////
    Optional<Node::Ref<Node::IfStatement>> Parser::ParseIfStatement()
	{
		/////////////////////////////////////////////////////////////////
		// If statement
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::If))
		{
			Node::Ref<Node::IfStatement> ifStatement = m_Tracker.New<Node::IfStatement>();

			Consume(); // If token

			CheckConsume(TokenType::OpenParenthesis, "Expected `(`.");

			if (auto expr = ParseExpression())
			{
				if (!TypeCollection::ImplicitCastable(expr.Value()->GetType(), Type(TypeSpecifier::Bool)))
					Compiler::Error(Peek(-1).Value().LineNumber, "Condition branch's expression must be a boolean type.");

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



	Optional<Node::Ref<Node::ScopeStatement>> Parser::ParseScopeStatement(bool startScope)
	{
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
		{
			Node::Ref<Node::ScopeStatement> scope = m_Tracker.New<Node::ScopeStatement>();
			
			Consume(); // '{' token

			if (startScope)
				m_Scopes.BeginScope();

			while (auto statement = ParseStatement())
				scope->Statements.push_back(statement.Value());

			{
				Optional<size_t> returnIndex = scope->GetReturnStatementIndex();
				if (returnIndex.HasValue() && (returnIndex.Value() + 1) != scope->Statements.size()) // Remove trailing code after a return statement
				{
					Compiler::Warn(Peek(-1).Value().LineNumber, "Found redundant code after return statement. Code will never be reached.");
					scope->Statements.resize(returnIndex.Value());
				}
			}

			m_Scopes.EndScope();

			CheckConsume(TokenType::CloseCurlyBrace, "Expected `}}`");
			return m_Tracker.Return<Node::ScopeStatement>();
		}

		return {};
	}



	Optional<Node::Ref<Node::VariableStatement>> Parser::ParseVariableStatement()
	{
		size_t offset = 0;
		
		/////////////////////////////////////////////////////////////////
		// Variable initiazation
		/////////////////////////////////////////////////////////////////
		if (PeekIsType(offset) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Equals))
		{
			Node::Ref<Node::VariableStatement> variable = m_Tracker.New<Node::VariableStatement>();

			Optional<Type> type = GetType();
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type for variable.");
				return {};
			}

			variable->VariableType = type.Value();
			variable->Variable = Consume().Value; // Identifier token

			Consume(); // '=' token
			
			if (m_Scopes.Exists(variable->Variable))
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Cannot redefine a variable.");

				ParseExpression(); // Parse the upcoming expression so we continue properly
				CheckConsume(TokenType::Semicolon, "Expected `;`.");

				m_Tracker.Pop<Node::VariableStatement>();
				return {};
			}

			m_Scopes.PushVar(variable->Variable, type.Value());

			// Expression resolution
			if (auto expr = ParseExpression())
			{
				if (!TypeCollection::ImplicitCastable(expr.Value()->GetType(), type.Value()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Variable creation of \"{0}\" expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", variable->Variable, TypeCollection::ToString(type.Value()), TypeCollection::ToString(expr.Value()->GetType()));

					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return m_Tracker.Return<Node::VariableStatement>();
				}
				// Note: In the future is should set the array sizes recursively for arrays with multiple directions.
				else if (type.Value().IsArray() && type.Value().GetArraySize().empty() && expr.Value()->GetType().IsArray())
				{
					variable->VariableType.SetArraySize(expr.Value()->GetType().GetArraySize());
				}

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
			Node::Ref<Node::VariableStatement> variable = m_Tracker.New<Node::VariableStatement>();

			Optional<Type> type = GetType();
			if (!type.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type for variable.");

				CheckConsume(TokenType::Semicolon, "Expected `;`.");
				m_Tracker.Pop<Node::VariableStatement>();
				return {};
			}
			else if (!type.Value().IsMut())
			{
				Compiler::Warn(Peek(0).Value().LineNumber, "Initializing immutable variable with default value.");
			}

			variable->VariableType = type.Value();
			variable->Variable = Consume().Value; // Identifier token

			m_Scopes.PushVar(variable->Variable, type.Value());

			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			return m_Tracker.Return<Node::VariableStatement>();
		}

		return {};
	}



	Optional<Node::Ref<Node::Statement>> Parser::ParseStatement()
	{
		/////////////////////////////////////////////////////////////////
		// Scope
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::OpenCurlyBrace))
		{
			if (auto scopeStatement = ParseScopeStatement())
			{
				Node::Ref<Node::Statement> statement = m_Tracker.New<Node::Statement>();
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
			Node::Ref<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			statement->StatementObj = ifStatement.Value();

			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Variable creation
		/////////////////////////////////////////////////////////////////
		else if (auto varStatement = ParseVariableStatement())
		{
			Node::Ref<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			statement->StatementObj = varStatement.Value();

			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Function call
		/////////////////////////////////////////////////////////////////
		else if (auto funcCall = ParseFunctionCall())
		{
			Node::Ref<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			statement->StatementObj = funcCall.Value();

			CheckConsume(TokenType::Semicolon, "Expected `;`.");
			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Return
		/////////////////////////////////////////////////////////////////
		else if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Return))
		{
			Node::Ref<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			
			auto returnStatement = Node::New<Node::ReturnStatement>();
			statement->StatementObj = returnStatement;

			Consume(); // Return token

			if (auto expr = ParseExpression())
			{
				returnStatement->Expr = expr.Value();
				
				// Check if returnStatement matches the returnType of the function
				// or is castable to the proper returnType.
				if (!TypeCollection::ImplicitCastable(expr.Value()->GetType(), m_Tracker.Get<Node::FunctionDefinition>()->GetType()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Function '{0}' expects type {1} as return type, instead it got {2}, {2} is not castable to {1}.", m_Tracker.Get<Node::FunctionDefinition>()->Name, TypeCollection::ToString(m_Tracker.Get<Node::FunctionDefinition>()->GetType()), TypeCollection::ToString(expr.Value()->GetType()));

					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					m_Tracker.Pop<Node::Statement>();
					return {};
				}
			}
			else if (m_Tracker.Get<Node::FunctionDefinition>() != (Node::Ref<Node::FunctionDefinition>)Node::NullRef &&
				m_Tracker.Get<Node::FunctionDefinition>()->GetType() != Type(TypeSpecifier::Void))
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Function '{0}' expects type {1} as return type, instead it got void, void is not castable to {1}.", m_Tracker.Get<Node::FunctionDefinition>()->Name, TypeCollection::ToString(m_Tracker.Get<Node::FunctionDefinition>()->GetType()));

				CheckConsume(TokenType::Semicolon, "Expected `;`.");
				m_Tracker.Pop<Node::Statement>();
				return {};
			}
			// A invalid expression / no expression is valid for void functions
			
			CheckConsume(TokenType::Semicolon, "Expected `;`.");

			return m_Tracker.Return<Node::Statement>();
		}

		/////////////////////////////////////////////////////////////////
		// Variable assignment
		/////////////////////////////////////////////////////////////////
		else if (auto variable = ParseExpression())
		{
			// If it's not an assignment, I don't know what to do.
			if (!Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Equals))
				return {};

			Node::Ref<Node::Statement> statement = m_Tracker.New<Node::Statement>();
			auto assignment = Node::New<Node::AssignmentStatement>();

			statement->StatementObj = assignment;

			assignment->Variable = variable.Value(); 

			Consume(); // '=' token

			// Checks
			{
				if (!variable.Value()->IsLValue() && variable.Value()->GetUnderlyingType() != Node::NodeType::IdentifierTerm)
					Compiler::Error(Peek(0).Value().LineNumber, "Cannot assign value to an RValue.");
				if (!assignment->Variable->GetType().IsMut())
					Compiler::Error(Peek(0).Value().LineNumber, "Cannot assign value to an immutable variable.");
			}

			if (auto expr = ParseExpression())
			{
				if (!TypeCollection::ImplicitCastable(expr.Value()->GetType(), assignment->Variable->GetType()))
				{
					Compiler::Error(Peek(0).Value().LineNumber, "Cannot assign type {0} to {1}.", TypeCollection::ToString(expr.Value()->GetType()), TypeCollection::ToString(assignment->Variable->GetType()));

					CheckConsume(TokenType::Semicolon, "Expected `;`.");
					return m_Tracker.Return<Node::Statement>();
				}

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
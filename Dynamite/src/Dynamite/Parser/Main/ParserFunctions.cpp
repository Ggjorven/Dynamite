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
	// Function parsing
	/////////////////////////////////////////////////////////////////
	Optional<Node::Reference<Node::FunctionCall>> Parser::ParseFunctionCall()
	{
		/////////////////////////////////////////////////////////////////
		// FunctionCall
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::OpenParenthesis))
		{
			// Parse name & returnType
			Node::Reference<Node::FunctionCall> function = m_Tracker.New<Node::FunctionCall>();
			function->Function = Consume();
			
			Consume(); // '(' token

			Optional<Type> returnType = FunctionSystem::GetReturnType(function->Function.Value);
			if (!returnType.HasValue())
			{
				Compiler::Error(Peek(-2).Value().LineNumber, "Undefined symbol: {0}", function->Function.Value);
				m_Tracker.Pop<Node::FunctionCall>();
				return {};
			}

			function->ReturnType = returnType.Value();

			// Parse arguments // TODO: Add default args and handle it here
			while (true)
			{
				size_t index = 0;

				if (auto argument = ParseExpression())
				{
					Optional<Type> symbolArgType = FunctionSystem::GetArgumentType(function->Function.Value, index);
					if (!symbolArgType.HasValue())
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Passed in {0} arguments, when the symbol for '{1}' doesn't match.", index + 1, function->Function.Value);
						m_Tracker.Pop<Node::FunctionCall>();
						return {};
					}

					if (!TypeSystem::Castable(argument.Value()->GetType(), symbolArgType.Value()))
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Argument {0} expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", index, TypeSystem::ToString(symbolArgType.Value()), TypeSystem::ToString(argument.Value()->GetType()));

						CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
						return m_Tracker.Return<Node::FunctionCall>();
					}

					// Note: Only casts if the internal type is a literalterm
					Cast(argument.Value()->GetType(), symbolArgType.Value(), argument.Value());
					function->Arguments.push_back(argument.Value());
					index++;
				}
				else
					break;
			}

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			return m_Tracker.Return<Node::FunctionCall>();
		}

		return {};
	}



	Optional<Node::Reference<Node::Function>> Parser::ParseFunction()
	{
		size_t offset = 0;
		
		/////////////////////////////////////////////////////////////////
		// Function
		/////////////////////////////////////////////////////////////////
		if (PeekIsType(offset) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::OpenParenthesis))
		{
			Node::Reference<Node::Function> func = m_Tracker.New<Node::Function>(); 
			
			Optional<Type> returnType = GetType();
			if (!returnType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function return type.");
				m_Tracker.Pop<Node::Function>();
				return {};
			}

			func->ReturnType = returnType.Value();
			func->Name = Consume(); // Identifier token

			Consume(); // '(' token

			std::vector<Type> parameterTypes = { };
			while (true)
			{
				size_t parameterOffset = 0;

				if (PeekIsType(parameterOffset) &&
					Utils::OptMemberIs(Peek(parameterOffset++), &Token::Type, TokenType::Identifier))
				{
					Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>();
					
					Optional<Type> variableType = GetType();
					if (!variableType.HasValue())
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function parameter.");
						return m_Tracker.Return<Node::Function>();
					}

					parameterTypes.push_back(variableType.Value());

					variable->VariableType = variableType.Value();
					variable->Variable = Consume(); // Identifier token

					// Note: We begin the scope here so we can 
					// expose the parameters to the scope as well.
					ScopeSystem::BeginScope();

					ScopeSystem::PushVar(variable->Variable.Value, variableType.Value());

					// Check if it has a default value
					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Equals))
					{
						Consume(); // '=' token

						if (auto expr = ParseExpression())
							variable->Expr = expr.Value();
						else
							Compiler::Error("Expected expression.");
					}

					func->Parameters.push_back(variable);

					// Continue if the next is a comma
					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Comma))
					{
						Consume(); // ',' token
						continue;
					}
				}
				else
					break;
			}

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");

			FunctionSystem::Add(func->Name.Value, func->GetType(), parameterTypes);

			if (auto body = ParseScopeStatement(false))
				func->Body = body.Value();
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Expected scope.");

			if (!TypeSystem::IsVoid(returnType.Value()))
			{
				if (func->Body->Statements.empty() || !std::holds_alternative<Node::Reference<Node::ReturnStatement>>(func->Body->Statements.back()->StatementObj))
					Compiler::Warn(Peek(-2).Value().LineNumber, "Function does not end with return statement and return type != void.");;
			}

			return m_Tracker.Return<Node::Function>();
		}

		return {};
	}

}
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

			// Parse arguments
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
			
			// Check if argument count matches.
			if (function->Arguments.size() != FunctionSystem::GetRequiredArgCount(function->Function.Value))
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Function '{0}' expects {1} argument(s), got {2}.", function->Function.Value, FunctionSystem::GetRequiredArgCount(function->Function.Value), function->Arguments.size());

				CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
				return m_Tracker.Return<Node::FunctionCall>();
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
			Node::Reference<Node::Function> func = Node::New<Node::Function>();
			
			Optional<Type> returnType = GetType();
			if (!returnType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function return type.");
				m_Tracker.Pop<Node::FunctionDefinition>();
				return {};
			}

			Token name = Consume(); // Identifier token
        	std::vector<Node::Reference<Node::VariableStatement>> parameters = { };

			Consume(); // '(' token

			bool hasDefaultArguments = false;
			std::vector<Type> parameterTypes = { };
			std::vector<std::pair<Type, bool>> parameterTypesAndDefault = { };
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
						return {};
					}

					variable->VariableType = variableType.Value();
					variable->Variable = Consume(); // Identifier token

					// Check if it has a default value
					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Equals))
					{
						Consume(); // '=' token

						if (auto expr = ParseExpression())
							variable->Expr = expr.Value();
						else
							Compiler::Error("Expected expression.");
					}

					parameters.push_back(variable);

					hasDefaultArguments |= (variable->Expr == ((Node::Reference<Node::Expression>)Node::NullRef) ? false : true);
					parameterTypes.emplace_back(variable->VariableType);
					parameterTypesAndDefault.emplace_back(variable->VariableType, ((variable->Expr == (Node::Reference<Node::Expression>)Node::NullRef) ? true : false));

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

			if (!FunctionSystem::Exists(name.Value, returnType.Value(), parameterTypes))
				FunctionSystem::Add(name.Value, returnType.Value(), parameterTypesAndDefault);

			/////////////////////////////////////////////////////////////////
			// Declaration
			/////////////////////////////////////////////////////////////////
			if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Semicolon))
			{
				Consume(); // ';' token

				Node::Reference<Node::FunctionDeclaration> declaration = m_Tracker.New<Node::FunctionDeclaration>();
				declaration->ReturnType = returnType.Value();
				declaration->Name = name;
				declaration->Parameters = parameters;

				func->Func = declaration;

				m_Tracker.Pop<Node::FunctionDeclaration>();
			}
			/////////////////////////////////////////////////////////////////
			// Definition
			/////////////////////////////////////////////////////////////////
			else
			{
				Node::Reference<Node::FunctionDefinition> definition = m_Tracker.New<Node::FunctionDefinition>();
				definition->ReturnType = returnType.Value();
				definition->Name = name;
				definition->Parameters = parameters;

				// Check if the declaration matches the definition
				if (FunctionSystem::Exists(definition->Name.Value, definition->ReturnType, parameterTypes))
				{
					if (hasDefaultArguments)
					{
						Compiler::Error(Peek(-1).Value().LineNumber, "Cannot redefine default arguments.");
					}
				}

				// Note: We begin the scope here so we can 
				// expose the parameters to the scope as well.
				ScopeSystem::BeginScope();

				// Expose parameters
				for (const auto& parameter : parameters)
					ScopeSystem::PushVar(parameter->Variable.Value, parameter->GetType());

				// Parse the scope
				if (auto body = ParseScopeStatement(false))
					definition->Body = body.Value();
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Expected function body.");

				// Check for ending return statement
				if (!TypeSystem::IsVoid(returnType.Value()))
				{
					if (definition->Body->Statements.empty() || !std::holds_alternative<Node::Reference<Node::ReturnStatement>>(definition->Body->Statements.back()->StatementObj))
						Compiler::Warn(Peek(-2).Value().LineNumber, "Function does not end with return statement and return type != void.");;
				}

				func->Func = definition;

				m_Tracker.Pop<Node::FunctionDefinition>();
			}

			return func;
		}

		return {};
	}

}
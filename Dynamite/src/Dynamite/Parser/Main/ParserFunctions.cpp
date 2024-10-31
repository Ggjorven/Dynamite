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

			if (!FunctionSystem::Exists(function->Function.Value))
			{
				Compiler::Error(Peek(-2).Value().LineNumber, "Undefined symbol: {0}", function->Function.Value);
				m_Tracker.Pop<Node::FunctionCall>();
				return {};
			}

			// Parse arguments
			while (true)
			{
				size_t index = 0;

				if (auto argument = ParseExpression())
				{
					std::vector<Type> symbolArgTypes = FunctionSystem::GetArgumentTypes(function->Function.Value, index);
					if (symbolArgTypes.empty())
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Passed in {0} arguments, when the symbol for '{1}' doesn't match.", index + 1, function->Function.Value);
						m_Tracker.Pop<Node::FunctionCall>();
						return {};
					}

					function->Arguments.push_back(argument.Value());

					// Continue if the next is a comma
					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Comma))
					{
						Consume(); // ',' token
						continue;
					}
					index++;
				}
				else
					break;
			}

			const FunctionSystem::Function& functionDefinitions = *FunctionSystem::GetFunction(function->Function.Value);

			size_t functionOverload = Pulse::Numeric::Max<size_t>();
			for (size_t overloadIndex = 0; overloadIndex < functionDefinitions.Overloads.size(); overloadIndex++)
			{
				const auto& overload = functionDefinitions.Overloads[overloadIndex];

				if ((overload.Parameters.size() < FunctionSystem::GetRequiredArgCounts(function->Function.Value)[overloadIndex]) ||
					(overload.Parameters.size() > FunctionSystem::GetArgCounts(function->Function.Value)[overloadIndex]))
					continue;

				bool continueLoop = false;
				for (size_t parameterIndex = 0; parameterIndex < overload.Parameters.size(); parameterIndex++)
				{
					if (function->Arguments.size() <= parameterIndex || !TypeSystem::Castable(function->Arguments[parameterIndex]->GetType(), overload.Parameters[parameterIndex].first))
					{
						continueLoop = true;
						break;
					}
				}

				if (continueLoop)
					continue;

				functionOverload = overloadIndex;
			}

			if (functionOverload == Pulse::Numeric::Max<size_t>())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Function call does not match any of the symbols.");
				m_Tracker.Pop<Node::FunctionCall>();
				return {};
			}

			function->ReturnType = FunctionSystem::GetReturnTypes(function->Function.Value)[functionOverload];

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

				size_t a = m_Index;

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
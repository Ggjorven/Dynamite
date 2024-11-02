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
	// Function parsing
	/////////////////////////////////////////////////////////////////
	Optional<Node::Ref<Node::FunctionCall>> Parser::ParseFunctionCall()
	{
		/////////////////////////////////////////////////////////////////
		// FunctionCall
		/////////////////////////////////////////////////////////////////
		if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(1), &Token::Type, TokenType::OpenParenthesis))
		{
			// Parse name & returnType
			Node::Ref<Node::FunctionCall> function = m_Tracker.New<Node::FunctionCall>();
			function->Function = Consume().Value;
			
			Consume(); // '(' token

			if (!m_Functions.Exists(function->Function))
			{
				Compiler::Error(Peek(-2).Value().LineNumber, "Undefined symbol: {0}", function->Function);
				m_Tracker.Pop<Node::FunctionCall>();
				return {};
			}

			// Parse arguments
			while (true)
			{
				size_t index = 0;

				if (auto argument = ParseExpression())
				{
					std::vector<Type> symbolArgTypes = m_Functions.GetArgumentTypes(function->Function, index);
					if (symbolArgTypes.empty())
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Passed in {0} arguments, when the symbol for '{1}' doesn't match.", index + 1, function->Function);
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

			const ParserFunction& functionDefinitions = *m_Functions.GetFunction(function->Function);

			size_t functionOverload = Pulse::Numeric::Max<size_t>();
			for (size_t overloadIndex = 0; overloadIndex < functionDefinitions.Overloads.size(); overloadIndex++)
			{
				const auto& overload = functionDefinitions.Overloads[overloadIndex];

				if ((overload.Parameters.size() < m_Functions.GetRequiredArgCounts(function->Function)[overloadIndex]) ||
					(overload.Parameters.size() > m_Functions.GetArgCounts(function->Function)[overloadIndex]))
					continue;

				bool continueLoop = false;
				for (size_t parameterIndex = 0; parameterIndex < overload.Parameters.size(); parameterIndex++)
				{
					if (function->Arguments.size() <= parameterIndex || !TypeCollection::ImplicitCastable(function->Arguments[parameterIndex]->GetType(), overload.Parameters[parameterIndex].first))
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

			function->ReturnType = m_Functions.GetReturnTypes(function->Function)[functionOverload];
			function->OverloadIndex = functionOverload;

			CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
			return m_Tracker.Return<Node::FunctionCall>();
		}

		return {};
	}



	Optional<Node::Ref<Node::Function>> Parser::ParseFunction()
	{
		size_t offset = 0;
		
		/////////////////////////////////////////////////////////////////
		// Function
		/////////////////////////////////////////////////////////////////
		if (PeekIsType(offset) && 
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::Identifier) &&
			Utils::OptMemberIs(Peek(offset++), &Token::Type, TokenType::OpenParenthesis))
		{
			Node::Ref<Node::Function> func = Node::New<Node::Function>();
			
			Optional<Type> returnType = GetType();
			if (!returnType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function return type.");
				m_Tracker.Pop<Node::FunctionDefinition>();
				return {};
			}

			Token name = Consume(); // Identifier token
        	std::vector<Node::Ref<Node::VariableStatement>> parameters = { };

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
					Node::Ref<Node::VariableStatement> variable = Node::New<Node::VariableStatement>();
					
					Optional<Type> variableType = GetType();
					if (!variableType.HasValue())
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function parameter.");
						return {};
					}

					variable->VariableType = variableType.Value();
					variable->Variable = Consume().Value; // Identifier token

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

					hasDefaultArguments |= (variable->Expr == ((Node::Ref<Node::Expression>)Node::NullRef) ? false : true);
					parameterTypes.emplace_back(variable->VariableType);
					parameterTypesAndDefault.emplace_back(variable->VariableType, ((variable->Expr == (Node::Ref<Node::Expression>)Node::NullRef) ? true : false));

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

			if (!m_Functions.Exists(name.Value, returnType.Value(), parameterTypes))
				m_Functions.Add(name.Value, returnType.Value(), parameterTypesAndDefault);

			/////////////////////////////////////////////////////////////////
			// Declaration
			/////////////////////////////////////////////////////////////////
			if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Semicolon))
			{
				Consume(); // ';' token

				Node::Ref<Node::FunctionDeclaration> declaration = m_Tracker.New<Node::FunctionDeclaration>();
				declaration->ReturnType = returnType.Value();
				declaration->Name = name.Value;
				declaration->Parameters = parameters;

				func->Func = declaration;

				m_Tracker.Pop<Node::FunctionDeclaration>();
			}
			/////////////////////////////////////////////////////////////////
			// Definition
			/////////////////////////////////////////////////////////////////
			else
			{
				Node::Ref<Node::FunctionDefinition> definition = m_Tracker.New<Node::FunctionDefinition>();
				definition->ReturnType = returnType.Value();
				definition->Name = name.Value;
				definition->Parameters = parameters;

				// Check if the declaration matches the definition
				if (m_Functions.Exists(definition->Name, definition->ReturnType, parameterTypes))
				{
					if (hasDefaultArguments)
					{
						Compiler::Error(Peek(-1).Value().LineNumber, "Cannot redefine default arguments.");
					}
				}

				// Note: We begin the scope here so we can 
				// expose the parameters to the scope as well.
				m_Scopes.BeginScope();

				// Expose parameters
				for (const auto& parameter : parameters)
					m_Scopes.PushVar(parameter->Variable, parameter->GetType());

				// Parse the scope
				if (auto body = ParseScopeStatement(false))
					definition->Body = body.Value();
				else
					Compiler::Error(Peek(0).Value().LineNumber, "Expected function body.");

				// Check for ending return statement
				if (!returnType.Value().IsVoid())
				{
					if (definition->Body->Statements.empty() || !std::holds_alternative<Node::Ref<Node::ReturnStatement>>(definition->Body->Statements.back()->StatementObj))
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
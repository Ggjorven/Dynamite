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
			auto consumed = Consume(); // Name

			Consume(); // '(' token

			Optional<Type> returnType = FunctionSystem::GetReturnType(consumed.Value);
			if (!returnType.HasValue())
			{
				Compiler::Error(Peek(-2).Value().LineNumber, "Undefined symbol: {0}", consumed.Value);
				return {};
			}

			Node::Reference<Node::FunctionCall> function = Node::New<Node::FunctionCall>(returnType.Value(), consumed);

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
						return {};
					}

					if (!TypeSystem::Castable(argument.Value()->GetType(), symbolArgType.Value()))
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Argument {0} expects expression of type: {1}, but got {2}, {2} is not castable to {1}.", index, TypeSystem::ToString(symbolArgType.Value()), TypeSystem::ToString(argument.Value()->GetType()));

						// Parenthesis `)` resolution
						CheckConsume(TokenType::CloseParenthesis, "Expected `)`.");
						return function;
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

			return function;
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
			Optional<Type> returnType = GetType();
			if (!returnType.HasValue())
			{
				Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function return type.");
				return {};
			}

			Node::Reference<Node::Function> func = Node::New<Node::Function>(returnType.Value(), Consume()); // Identifier token

			std::string funcName = func->Name.Value;

			Consume(); // '(' token

			while (true)
			{
				size_t parameterOffset = 0;

				if (PeekIsType(parameterOffset) &&
					Utils::OptMemberIs(Peek(parameterOffset++), &Token::Type, TokenType::Identifier))
				{
					Optional<Type> variableType = GetType();
					if (!variableType.HasValue())
					{
						Compiler::Error(Peek(0).Value().LineNumber, "Invalid type as function parameter.");
						return func;
					}

					Node::Reference<Node::VariableStatement> variable = Node::New<Node::VariableStatement>(variableType.Value(), Consume()); // Identifier token

					std::string varName = variable->Variable.Value;

					// Note: We begin the scope here so we can 
					// expose the parameters to the scope as well.
					ScopeSystem::BeginScope();

					ScopeSystem::PushVar(varName, variableType.Value());

					if (Utils::OptMemberIs(Peek(0), &Token::Type, TokenType::Equals))
					{
						Consume(); // '=' token

						if (auto expr = ParseExpression())
							variable->Expr = expr.Value();
						else
							Compiler::Error("Expected expression.");
					}

					func->Parameters.push_back(variable);

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

			std::vector<Type> parameterTypes = { };
			for (const auto& parameter : func->Parameters)
				parameterTypes.push_back(parameter->GetType());

			FunctionSystem::Add(func->Name.Value, func->GetType(), parameterTypes);

			if (auto body = ParseScopeStatement(false))
				func->Body = body.Value();
			else
				Compiler::Error(Peek(0).Value().LineNumber, "Expected scope.");

			if (!TypeSystem::IsVoid(returnType.Value()))
			{
				if (func->Body->Statements.empty() || !std::holds_alternative<Node::Reference<Node::ReturnStatement>>(func->Body->Statements.back()->StatementObj))
				{
					Compiler::Warn(Peek(-2).Value().LineNumber, "Function does not end with return statement and return type != void.");;
				}
			}

			return func;
		}

		return {};
	}

}
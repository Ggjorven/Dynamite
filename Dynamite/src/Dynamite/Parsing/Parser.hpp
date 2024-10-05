#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Nodes.hpp"

#include <Pulse/Memory/ArenaAllocator.hpp>

#include <cstdint>
#include <vector>
#include <optional>
#include <unordered_map>

namespace Dynamite
{

	class Parser
	{
	public:
		explicit Parser(const std::vector<Token>& tokens);
		~Parser() = default;

		Nodes::Program GetProgram();

		static void Print(const Nodes::Program& program);

	public:
		std::optional<Nodes::Expression*> ParseExpression();
		std::optional<Nodes::Statement*> ParseStatement();

	private:
		// Returns the Token at m_Index + offset, if it is out of bounds it
		// will return an optional with no value. Checkable with .has_value()
		[[nodiscard]] std::optional<Token> Peek(size_t offset = 0) const;

		// Increments the index and returns the Token at m_Index
		Token Consume();
		Token TryConsume(TokenType tokenType, const std::string& msg = {});
		std::optional<Token> TryConsume(TokenType tokenType);

	private:
		const std::vector<Token>& m_Tokens;
		size_t m_Index = 0;

		std::unordered_map<std::string, Nodes::VariableType> m_IdentifierTypes = { };
	};

}
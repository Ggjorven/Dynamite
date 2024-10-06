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

		Node::Program GetProgram();

		static void Print(const Node::Program& program);

	public:
		std::optional<Node::Reference<Node::TermExpr>> ParseTermExpr();
		std::optional<Node::Reference<Node::Expression>> ParseExpr();
		std::optional<Node::Reference<Node::Statement>> ParseStatement();

	private:
		// Returns the Token at m_Index + offset, if it is out of bounds it
		// will return an optional with no value. Checkable with .has_value()
		[[nodiscard]] std::optional<Token> Peek(size_t offset = 0) const;

		// Increments the index and returns the Token at m_Index
		Token Consume();
		Token CheckConsume(TokenType tokenType, const std::string& msg = {});
		std::optional<Token> TryConsume(TokenType type);

		std::optional<Token> TryConsumeLiteral();

		bool PeekIsVariableType();
		bool PeekIsBinaryOperator();

		ValueType GetValueType(TokenType literalType, const std::string& value);

	private:
		const std::vector<Token>& m_Tokens;
		size_t m_Index = 0;

		// TODO: Add support for scopes
		std::unordered_map<std::string, ValueType> m_SymbolTypes = { };
	};

}
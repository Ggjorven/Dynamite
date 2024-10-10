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
		Parser(std::vector<Token>& tokens);
		~Parser() = default;

		Node::Program GetProgram();

		inline const std::vector<Token>& GetTokens() const { return m_Tokens; }
		inline const size_t GetIndex() const { return m_Index; }

	public:
		std::optional<Node::Reference<Node::TermExpr>> ParseTermExpr();
		std::optional<Node::Reference<Node::Expression>> ParseExpr(size_t minimumPrecedence = 0);
		std::optional<Node::Reference<Node::ScopeStatement>> ParseScope();
		std::optional<Node::Reference<Node::Statement>> ParseStatement();

	public:
		// Returns the Token at m_Index + offset, if it is out of bounds it
		// will return an optional with no value. Checkable with .has_value()
		[[nodiscard]] std::optional<Token> Peek(size_t offset = 0) const;

	private:
		// Increments the index and returns the Token at m_Index
		Token Consume();
		Token CheckConsume(TokenType tokenType, const std::string& msg = {});
		std::optional<Token> TryConsume(TokenType type);

		std::optional<Token> TryConsumeLiteral();

		bool PeekIs(const std::vector<TokenType>& allowedValues);
		bool PeekIsVariableType();
		bool PeekIsBinaryOperator();

		// Note: Only casts if the internal type is a literalterm
		void CastInternalValue(ValueType from, ValueType to, Node::Reference<Node::Expression> expression);

		inline uint32_t GetLineNumber() const { return Peek(0).value().LineNumber; }

	private:
		std::vector<Token>& m_Tokens;
		size_t m_Index = 0;

		// TODO: Add support for scopes
		std::vector<Variable> m_Variables = {};
		std::vector<size_t> m_Scopes = {};

		std::unordered_map<std::string, ValueType> m_SymbolTypes = { };
	};

}
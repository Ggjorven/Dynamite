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
		std::optional<Node::Reference<Node::Expression>> ParseExpr(const size_t minimumPrecedence = 0);
		std::optional<Node::Reference<Node::ScopeStatement>> ParseScope();
		std::optional<Node::Reference<Node::ConditionBranch>> ParseConditionBrach();
		std::optional<Node::Reference<Node::VariableStatement>> ParseVariable();
		std::optional<Node::Reference<Node::Statement>> ParseStatement();

		std::optional<Node::Reference<Node::Function>> ParseFunction();

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

		bool PeekIs(const std::vector<TokenType>& allowedValues) const;
		bool PeekIsValueType() const;
		bool PeekIsBinaryOperator() const;

		// Note: Only casts if the internal type is a literalterm
		void CastInternalValue(ValueType from, ValueType to, Node::Reference<Node::Expression> expression);

		inline uint32_t GetLineNumber() const { return (Peek(0).has_value() ? Peek(0).value().LineNumber : Peek(-1).value().LineNumber); }

	private:
		void PushVar(const std::string& name, ValueType type);
		void PushOffsetVar(const std::string& name, ValueType type); // Used for parameter variables
		void PopVar(size_t count);
		Variable GetVar(const std::string& name);

	private:
		std::vector<Token>& m_Tokens;
		size_t m_Index = 0;

		// Used for adding variables to function scope
		// For example parameters need to be available in the scope
		// but have to be declared outside, so we have an offset to 
		// allow these parameters to be accessed in the scope and still
		// be deleted afterwards.
		size_t m_VariableOffset = 0;

		std::vector<Variable> m_Variables = {};
		std::vector<size_t> m_Scopes = { 0 };
	};

}
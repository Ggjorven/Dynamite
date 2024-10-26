#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Parser/ScopeSystem.hpp"
#include "Dynamite/Parser/FunctionSystem.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

namespace Dynamite
{

	class Parser
	{
	public:
		// Constructors
		Parser(std::vector<Token>& tokens);
		~Parser() = default;

		// Public methods
		Node::Program GetProgram();

	public:
		// Parsing methods
		Optional<Node::Reference<Node::FunctionCall>>		ParseFunctionCall();
		Optional<Node::Reference<Node::TermExpr>>			ParseTermExpr();
		Optional<Node::Reference<Node::Expression>>			ParseExpression(size_t minimumPrecedence = 0);
		
		Optional<Node::Reference<Node::ConditionBranch>>	ParseConditionBrach();
		
		Optional<Node::Reference<Node::IfStatement>>		ParseIfStatement();
		Optional<Node::Reference<Node::ScopeStatement>>		ParseScopeStatement(bool startScope = true);
		Optional<Node::Reference<Node::VariableStatement>>	ParseVariableStatement();

		Optional<Node::Reference<Node::Statement>>			ParseStatement();
		Optional<Node::Reference<Node::Function>>			ParseFunction();

	private:
		// Peeking & Consuming
		Optional<Token> Peek(int64_t offset = 0) const;
		Token Consume();

		void CheckConsume(TokenType expect, const std::string& msg = {});

		bool PeekIsType(size_t& offset);
		Type GetType();

		void Cast(const Type& from, const Type& to, Node::Reference<Node::Expression> expression);

	private:
		std::vector<Token>& m_Tokens;
		size_t m_Index = 0;

		ScopeSystem m_Scopes;
		FunctionSystem m_Functions;
	};

}
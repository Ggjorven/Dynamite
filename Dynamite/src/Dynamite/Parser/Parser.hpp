#pragma once

#include "Dynamite/Core/Core.hpp"

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
		Optional<Node::Reference<Node::TermExpr>>			ParseTermExpr();
		Optional<Node::Reference<Node::Expression>>			ParseExpression(size_t minimumPrecedence = 0);
		
		Optional<Node::Reference<Node::ConditionBranch>>	ParseConditionBrach();
		
		Optional<Node::Reference<Node::IfStatement>>		ParseIfStatement();
		Optional<Node::Reference<Node::ScopeStatement>>		ParseScopeStatement();
		Optional<Node::Reference<Node::VariableStatement>>	ParseVariableStatement();

		Optional<Node::Reference<Node::Statement>>			ParseStatement();
		Optional<Node::Reference<Node::Function>>			ParseFunction();

	private:
		// Peeking & Consuming
		Optional<Token> Peek(size_t offset = 0) const;
		Token Consume();

	private:
		std::vector<Token>& m_Tokens;
		size_t m_Index = 0;
	};

}
#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Dynamite/Tokenizer/Token.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include "Dynamite/Dynamite/Parser/Collections/ParserState.hpp"

#include "Dynamite/Dynamite/Parser/Collections/ParserScopeCollection.hpp"
#include "Dynamite/Dynamite/Parser/Collections/ParserFunctionCollection.hpp"

namespace Dynamite
{

	class Parser
	{
	public:
		// Constructors
		Parser(std::vector<Token>& tokens);
		~Parser() = default;

		// Public methods
		Language::Node::Program GetProgram();

	public:
		// Parsing methods
		Optional<Language::Node::Ref<Language::Node::FunctionCall>>				ParseFunctionCall();
		Optional<Language::Node::Ref<Language::Node::TermExpr>>					ParseTermExpr();
		void																	HandleBinaryOperators(Language::Node::Ref<Language::Node::Expression>& expr, size_t minimumPrecedence);
		Optional<Language::Node::Ref<Language::Node::Expression>>				ParseExpression(size_t minimumPrecedence = 0);
		
		Optional<Language::Node::Ref<Language::Node::ConditionBranch>>			ParseConditionBrach();
		
		Optional<Language::Node::Ref<Language::Node::IfStatement>>				ParseIfStatement();
		Optional<Language::Node::Ref<Language::Node::ScopeStatement>>			ParseScopeStatement(bool startScope = true);
		Optional<Language::Node::Ref<Language::Node::VariableStatement>>		ParseVariableStatement();

		Optional<Language::Node::Ref<Language::Node::Statement>>				ParseStatement();
		Optional<Language::Node::Ref<Language::Node::Function>>					ParseFunction();

	private:
		// Peeking & Consuming
		Optional<Token> Peek(int64_t offset = 0) const;
		Token Consume();

		void CheckConsume(TokenType expect, const std::string& msg = {});

		bool PeekIsType(size_t& offset);
		Optional<Language::Type> GetType();

	private:
		std::vector<Token>& m_Tokens;
		size_t m_Index = 0;

		ParserState m_State = {};
	
		ParserScopeCollection m_Scopes = {};
		ParserFunctionCollection m_Functions = {};
	};

}
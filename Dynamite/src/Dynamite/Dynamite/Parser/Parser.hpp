#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Dynamite/Tokenizer/Token.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include "Dynamite/Dynamite/Parser/Nodes/ParseOperations.hpp"

namespace Dynamite
{

	class Parser
	{
	public:
		// Constructors
		Parser(std::vector<Token>& tokens);
		~Parser();

		// Public methods
		Language::Node::Program GetProgram();

		static Parser& Get();

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

	private:
	};

}
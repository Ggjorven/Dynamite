#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Tokens/Token.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Tokenizer
	/////////////////////////////////////////////////////////////////
	class Tokenizer
	{
	public:
		// Constructors
		Tokenizer(std::string& fileContent);
		~Tokenizer() = default;

		// Public methods
		std::vector<Token> Tokenize();

	public:
		// Peeking & consuming
		Optional<char> Peek(int64_t offset = 0) const;
		char Consume();

		// Handling functions
		bool HandleTypes(std::vector<Token>& tokens, std::string& buffer, uint32_t& line);
		bool HandleKeywords(std::vector<Token>& tokens, std::string& buffer, uint32_t& line);
		bool HandleOperators(std::vector<Token>& tokens, std::string& buffer, uint32_t& line);

	private:
		std::string& m_FileContent;
		size_t m_Index = 0;
	};

}
#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include <vector>
#include <functional>

namespace Dynamite
{

	// Note: Only support single file tokenization
	class Tokenizer
	{
	public:
		explicit Tokenizer(const std::string& fileContent);
		~Tokenizer() = default;

		std::vector<Token> GetTokens();

		// Debug print function
		static void Print(const std::vector<Token>& tokens);

	private:
		// Returns the char at m_Index + offset, if it is out of bounds it
		// will return an optional with no value. Checkable with .has_value()
		[[nodiscard]] std::optional<char> Peek(size_t offset = 1) const;

		// Increments the index and returns the character at m_Index
		char Consume();

	private:
		std::string const& m_FileContent;
		size_t m_Index = 0;
	};

}
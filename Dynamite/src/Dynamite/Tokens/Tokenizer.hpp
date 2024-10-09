#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
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
		static std::string FormatToken(const Token& token);
		static void Print(const std::vector<Token>& tokens);

	private:
		// Returns the char at m_Index + offset, if it is out of bounds it
		// will return an optional with no value. Checkable with .has_value()
		[[nodiscard]] std::optional<char> Peek(size_t offset = 0) const;

        template<typename Func, typename ...Args>
        [[nodiscard]] bool PeekCheck(Func&& func, Args&& ...args);

		// Increments the index and returns the character at m_Index
		char Consume();

		/////////////////////////////////////////////////////////////////
		bool HandleTypes(std::string& buffer, std::vector<Token>& tokens, uint32_t lineNumber);
		void HandleKeywords(std::string& buffer, std::vector<Token>& tokens, uint32_t lineNumber);
		bool HandleOperators(std::vector<Token>& tokens, uint32_t& lineNumber);

		/////////////////////////////////////////////////////////////////
		std::string GetCurrentLine(size_t* beginLineIndex = nullptr);

	private:
		std::string const& m_FileContent;
		size_t m_Index = 0;
	};

    template<typename Func, typename ...Args>
    bool Tokenizer::PeekCheck(Func&& func, Args&& ...args)
    {
        auto peeked = Peek();
        return peeked.has_value() && func(peeked.value(), std::forward<Args>(args)...);
    }

}

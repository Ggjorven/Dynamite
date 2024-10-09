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
		Tokenizer(std::string& fileContent);
		~Tokenizer() = default;

		std::vector<Token> Tokenize();

		// Getters
		inline const size_t GetIndex() const { return m_Index; }

		inline const std::vector<Token>& GetTokens() const { return m_Tokens; }
		inline const std::string& GetBuffer() const { return m_Buffer; }
		inline const uint32_t GetLineNumber() const { return m_LineNumber; }

	public:
		// Returns the char at m_Index + offset, if it is out of bounds it
		// will return an optional with no value. Checkable with .has_value()
		[[nodiscard]] std::optional<char> Peek(size_t offset = 0) const;

        template<typename Func, typename ...Args>
        [[nodiscard]] bool PeekCheck(Func&& func, Args&& ...args);

		// Increments the index and returns the character at m_Index
		char Consume();

		/////////////////////////////////////////////////////////////////
		bool HandleTypes();
		void HandleKeywords();
		bool HandleOperators();

	private:
		std::string& m_FileContent;
		size_t m_Index = 0;

		// Token usage
		std::vector<Token> m_Tokens = { };
		std::string m_Buffer = {};
		uint32_t m_LineNumber = 1;
	};

    template<typename Func, typename ...Args>
    bool Tokenizer::PeekCheck(Func&& func, Args&& ...args)
    {
        auto peeked = Peek();
        return peeked.has_value() && func(peeked.value(), std::forward<Args>(args)...);
    }

}

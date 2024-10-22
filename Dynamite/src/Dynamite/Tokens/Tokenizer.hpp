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

		// Methods
		std::vector<Token> Tokenize();

	public:
		// Private methodss
		Optional<char> Peek(size_t offset = 0) const;
		char Consume();

		bool PeekIs(size_t offset, char c) const;

		template<typename Func, typename ...Args>
		bool PeekCheck(Func&& func, Args&& ...args);

		// Handling functions
		bool HandleTypes(std::vector<Token>& tokens, std::string& buffer, uint32_t& line);
		bool HandleKeywords(std::vector<Token>& tokens, std::string& buffer, uint32_t& line);
		bool HandleOperators(std::vector<Token>& tokens, std::string& buffer, uint32_t& line);

	private:
		std::string& m_FileContent;
		size_t m_Index = 0;
	};

	/////////////////////////////////////////////////////////////////
	// Templated functions
	/////////////////////////////////////////////////////////////////
	template<typename Func, typename ...Args>
	inline bool Tokenizer::PeekCheck(Func&& func, Args && ...args)
	{
		Optional<char> value = Peek();
		return value.HasValue() && func(value.Value(), std::forward<Args>(args)...);
	}

}
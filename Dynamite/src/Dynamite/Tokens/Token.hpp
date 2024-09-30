#pragma once

#include <cstdint>

#include <string>
#include <optional>

namespace Dynamite
{

	enum class TokenType : uint16_t
	{
		None = 0,

		// Main
		Exit,					// Exit from Program
		Semicolon,				// End of 'statement'
		
		// Types
		IntegerLiteral,			// int32_t literal
	};

	struct Token
	{
	public:
		TokenType Type = TokenType::None;
		std::optional<std::string> Value = {};

		// Line number
		uint32_t Line = 0;
	};

}
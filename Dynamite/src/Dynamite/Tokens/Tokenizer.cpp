#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#define PeekCheck(func) Peek().has_value() && func(Peek().value())

namespace Dynamite
{
    Tokenizer::Tokenizer(const std::string& fileContent)
        : m_FileContent(fileContent)
    {
    }

    std::vector<Token> Tokenizer::GetTokens()
    {
        std::vector<Token> tokens = { };
        std::string buffer = {};

        while (Peek().has_value())
        {
            // Is alphabetic
            if (PeekCheck(std::isalpha))
            {
                buffer.push_back(Consume());

                // While is alphabetic or a number, keep reading
                while (PeekCheck(std::isalnum))
                    buffer.push_back(Consume());

                // Keywords/Functions
                if (buffer == "exit")
                {
                    tokens.push_back({ .Type = TokenType::Exit });
                }
                else // Invalid keyword or function name
                {
                    DY_LOG_ERROR("Invalid keyword or function name found at index: {0}, buffer = {1}", m_Index, buffer);
                }

                buffer.clear();
            }

            // Is number
            else if (PeekCheck(std::isdigit))
            {
                buffer.push_back(Consume());

                // While still a numeric value, keep reading
                while (PeekCheck(std::isdigit))
                    buffer.push_back(Consume());

                tokens.push_back({ .Type = TokenType::IntegerLiteral, .Value = buffer });
                buffer.clear();
            }

            // Semicolon
            else if (Peek().value() == ';')
            {
                Consume();
                tokens.push_back({ .Type = TokenType::Semicolon });
            }
            // Space
            else if (std::isspace(Peek().value()))
            {
                Consume();
            }

            // Invalid token
            else
            {
                DY_LOG_ERROR("Invalid token found at index: {0}, Buffer = {1}", m_Index, buffer);
                buffer.clear();
            }
        }

        m_Index = 0;
        return tokens;
    }

    void Tokenizer::Print(const std::vector<Token>& tokens)
    {
        for (const auto& token : tokens)
        {
            if (token.Value.has_value())
                DY_LOG_TRACE("({0}), TokenType::{1}, Value = {2}", token.Line, Pulse::Enum::Name(token.Type), token.Value.value());
            else
                DY_LOG_TRACE("({0}), TokenType::{1}", token.Line, Pulse::Enum::Name(token.Type));
        }
    }

    std::optional<char> Tokenizer::Peek(size_t offset) const
    {
        if (m_Index + offset >/*=*/ m_FileContent.length())
            return {};

        return m_FileContent.at(m_Index);
    }

    char Tokenizer::Consume()
    {
        return m_FileContent.at(m_Index++);
    }

}

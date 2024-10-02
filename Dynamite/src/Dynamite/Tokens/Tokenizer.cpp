#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#define PeekCheck(func) Peek().has_value() && func(Peek().value())

namespace Dynamite
{

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    Tokenizer::Tokenizer(const std::string& fileContent)
        : m_FileContent(fileContent)
    {
    }

    std::vector<Token> Tokenizer::GetTokens()
    {
        std::vector<Token> tokens = { };
        std::string buffer = {};
        uint32_t lineNumber = 1;

        while (Peek().has_value())
        {
            // Is alphabetic
            if (PeekCheck(std::isalpha))
            {
                buffer.push_back(Consume());

                // While is alphabetic or a number, keep reading
                while (PeekCheck(std::isalnum))
                    buffer.push_back(Consume());

                HandleKeywords(buffer, tokens, lineNumber);

                buffer.clear();
            }

            // Is number
            else if (PeekCheck(std::isdigit))
            {
                buffer.push_back(Consume());

                // While still a numeric value, keep reading
                while (PeekCheck(std::isdigit))
                    buffer.push_back(Consume());

                tokens.emplace_back(TokenType::Int64Literal, buffer, lineNumber);
                buffer.clear();
            }

            else if (HandleChars(tokens, lineNumber))
                continue;

            // Invalid token
            else
            {
                if (Peek().value() == '\n' || Peek().value() == '\r')
                    DY_LOG_ERROR("newline");

                DY_LOG_ERROR("Invalid token found at index: {0}, Buffer = {1}", m_Index, buffer);
                buffer.clear();
            }
        }

        m_Index = 0;
        return tokens;
    }

    std::string Tokenizer::FormatToken(const Token& token)
    {
        if (token.Value.has_value())
            return Pulse::Text::Format("({0}), TokenType::{1}, Value = {2}", token.LineNumber, Pulse::Enum::Name(token.Type), token.Value.value());
        else
            return Pulse::Text::Format("({0}), TokenType::{1}", token.LineNumber, Pulse::Enum::Name(token.Type));
    }

    void Tokenizer::Print(const std::vector<Token>& tokens)
    {
        for (const auto& token : tokens)
            DY_LOG_TRACE(FormatToken(token));
    }

    /////////////////////////////////////////////////////////////////
    // Peeking & consuming
    /////////////////////////////////////////////////////////////////
    std::optional<char> Tokenizer::Peek(size_t offset) const
    {
        if (m_Index + offset >= m_FileContent.size())
            return {};

        return m_FileContent.at(m_Index + offset);
    }

    char Tokenizer::Consume()
    {
        return m_FileContent.at(m_Index++);
    }

    /////////////////////////////////////////////////////////////////
    // Handling functions
    /////////////////////////////////////////////////////////////////
    bool Tokenizer::HandleKeywords(const std::string& buffer, std::vector<Token>& tokens, uint32_t lineNumber)
    {
        // Variable declaration
        if (buffer == "let")
        {
            tokens.emplace_back(TokenType::Let, lineNumber);
            return true;
        }
        // Exit function
        else if (buffer == "exit")
        {
            tokens.emplace_back(TokenType::Exit, lineNumber);
            return true;
        }

        else // Invalid keyword or function name
        {
            DY_LOG_ERROR("Invalid keyword or function name found at index: {0}, buffer = {1}", m_Index, buffer);
        }

        return false;
    }

    bool Tokenizer::HandleChars(std::vector<Token>& tokens, uint32_t& lineNumber)
    {
        // Semicolon
        if (Peek().value() == ';')
        {
            Consume();
            tokens.emplace_back(TokenType::Semicolon, lineNumber);
            return true;
        }
        // Open parenthesis
        else if (Peek().value() == '(')
        {
            Consume();
            tokens.emplace_back(TokenType::OpenParenthesis, lineNumber);
            return true;
        }
        // Close parenthesis
        else if (Peek().value() == ')')
        {
            Consume();
            tokens.emplace_back(TokenType::CloseParenthesis, lineNumber);
            return true;
        }
        // Equals
        else if (Peek().value() == '=')
        {
            Consume();
            tokens.emplace_back(TokenType::Equals, lineNumber);
            return true;
        }

        // Newline (for incrementing)
        else if (Peek().value() == '\n' || Peek().value() == '\r')
        {
            Consume();
            lineNumber++;
            return true;
        }
        // Space
        else if (std::isspace(Peek().value()))
        {
            Consume();
            return true;
        }

        return false;
    }

}

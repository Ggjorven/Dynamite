#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

    #define PeekCheck(func) Peek().has_value() && func(Peek().value())

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

                // If no type is found, try keywords.
                if (!HandleTypes(buffer, tokens, lineNumber))
                    HandleKeywords(buffer, tokens, lineNumber);
            }

            // Is number
            else if (PeekCheck(std::isdigit))
            {
                buffer.push_back(Consume());

                // While still a numeric value, keep reading
                while (PeekCheck(std::isdigit))
                    buffer.push_back(Consume());

                // TODO: check if there a '.' and the make it float literal

                tokens.emplace_back(TokenType::IntegerLiteral, buffer, lineNumber);
                buffer.clear();
            }

            // Handle operators and other chars
            else if (HandleChars(tokens, lineNumber))
                continue;

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
    bool Tokenizer::HandleTypes(std::string& buffer, std::vector<Token>& tokens, uint32_t lineNumber)
    {
        // Note: It's okay to use if instead of else if, since
        // we return from the function if it has been found.
        #define HandleType(tokenType)                           \
            if (buffer == Pulse::Enum::Name(tokenType))         \
            {                                                   \
                tokens.emplace_back(tokenType, lineNumber);     \
                buffer.clear();                                 \
                return true;                                    \
            }

        HandleType(TokenType::Int64);

        return false;
    }

    void Tokenizer::HandleKeywords(std::string& buffer, std::vector<Token>& tokens, uint32_t lineNumber)
    {
        // Exit function
        if (buffer == "exit")
        {
            tokens.emplace_back(TokenType::Exit, lineNumber);
            buffer.clear();
            return;
        }

        // Else we say its an Identifier
        tokens.emplace_back(TokenType::Identifier, buffer, lineNumber);
        buffer.clear();
    }

    bool Tokenizer::HandleChars(std::vector<Token>& tokens, uint32_t& lineNumber)
    {
        // Note: It's okay to use if instead of else if, since
        // we return from the function if it has been found.
        #define CharOperator(c, tokenType)                      \
            if (Peek().value() == c)                            \
            {                                                   \
                Consume();                                      \
                tokens.emplace_back(tokenType, lineNumber);     \
                return true;                                    \
            }

        // Signs
        CharOperator(';', TokenType::Semicolon);
        CharOperator('(', TokenType::OpenParenthesis);
        CharOperator(')', TokenType::CloseParenthesis);
        CharOperator('=', TokenType::Equals);

        // Operators
        CharOperator('+', TokenType::Plus);
        CharOperator('-', TokenType::Minus);
        CharOperator('*', TokenType::Star);
        CharOperator('/', TokenType::Divide);

        // Newline (for incrementing)
        if (Peek().value() == '\n' || Peek().value() == '\r')
        {
            Consume();
            lineNumber++;
            return true;
        }
        // Space
        if (std::isspace(Peek().value()))
        {
            Consume();
            return true;
        }

        return false;
    }

}

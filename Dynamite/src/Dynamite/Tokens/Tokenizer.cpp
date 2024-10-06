#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

    #define PeekCheck(func, ...) Peek().has_value() && func(Peek().value(), __VA_ARGS__)

    namespace
    {
        static bool IsAlpha(char c)
        {
            return (std::isalpha(c) || c == '_');
        }

        static bool IsNumeric(char c, bool allowsMinus = false)
        {
            return (std::isdigit(c) || (allowsMinus ? (c == '-') : false));
        }

        static bool IsAlphaNumeric(char c)
        {
            return IsAlpha(c) || IsNumeric(c);
        }
    }

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

        while (Peek(0).has_value())
        {
            // Is alphabetic
            if (PeekCheck(IsAlpha)) // Note: Also handles boolean values
            {
                buffer.push_back(Consume());

                // While is alphabetic or a number, keep reading
                while (PeekCheck(IsAlphaNumeric))
                    buffer.push_back(Consume());

                // If no type is found, try keywords.
                if (!HandleTypes(buffer, tokens, lineNumber))
                    HandleKeywords(buffer, tokens, lineNumber);
            }

            // Is number
            else if (PeekCheck(IsNumeric, true))
            {
                buffer.push_back(Consume());

                // While still a numeric value, keep reading
                while (PeekCheck(IsNumeric))
                    buffer.push_back(Consume());

                if (Peek(0).has_value() && (Peek(0).value() == '.'))
                {
                    buffer.push_back(Consume()); // '.' char

                    while (PeekCheck(IsNumeric))
                        buffer.push_back(Consume());

                    tokens.emplace_back(TokenType::FloatLiteral, buffer, lineNumber);
                    buffer.clear();
                }
                else // Else it's just an integer literal
                {
                    tokens.emplace_back(TokenType::IntegerLiteral, buffer, lineNumber);
                    buffer.clear();
                }
            }

            // Is char  
            else if (Peek(0).has_value() && Peek(0).value() == '\'' // Begin char character
                && Peek(2).has_value() && Peek(2).value() == '\'') // End char character
            {
                Consume(); // ''' Start char character
                buffer.push_back(Consume()); // Char character
                Consume(); // ''' End char character

                tokens.emplace_back(TokenType::CharLiteral, buffer, lineNumber);
                buffer.clear();
            }

            // Is string // Note: String buffer keeps '\'s
            else if (Peek(0).has_value() && Peek(0).value() == '"')
            {
                Consume(); // '"' Start string character

                bool contin = Peek(0).has_value() && Peek(0).value() != '"';
                while (contin)
                {
                    buffer.push_back(Consume());

                    if (Peek(0).has_value() && Peek(0).value() == '"' && Peek(-1).value() != '\\')
                        contin = false;
                }

                Consume(); // '"' End string character

                tokens.emplace_back(TokenType::StringLiteral, buffer, lineNumber);
                buffer.clear();
            }

            // Handle operators and other chars
            else if (HandleOperators(tokens, lineNumber))
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

        HandleType(TokenType::Bool);

        HandleType(TokenType::Int8);
        HandleType(TokenType::Int16);
        HandleType(TokenType::Int32);
        HandleType(TokenType::Int64);

        HandleType(TokenType::UInt8);
        HandleType(TokenType::UInt16);
        HandleType(TokenType::UInt32);
        HandleType(TokenType::UInt64);

        HandleType(TokenType::Float32);
        HandleType(TokenType::Float64);

        HandleType(TokenType::Char);
        HandleType(TokenType::String);

        return false;
    }

    void Tokenizer::HandleKeywords(std::string& buffer, std::vector<Token>& tokens, uint32_t lineNumber)
    {
        // Boolean values
        if (buffer == "false")
        {
            tokens.emplace_back(TokenType::BoolLiteral, "0", lineNumber);
            buffer.clear();
            return;
        }
        if (buffer == "true")
        {
            tokens.emplace_back(TokenType::BoolLiteral, "1", lineNumber);
            buffer.clear();
            return;
        }

        // Exit function // TODO: Make reusable for all functions
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

    bool Tokenizer::HandleOperators(std::vector<Token>& tokens, uint32_t& lineNumber)
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

        CharOperator('|', TokenType::Or);
        CharOperator('&', TokenType::And);
        CharOperator('^', TokenType::Xor);

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

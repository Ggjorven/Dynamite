#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Parsing/Variables.hpp"

#include "Dynamite/Compiler/CompilerSuite.hpp"

namespace Dynamite
{

    namespace
    {
        static bool IsAlpha(char c)
        {
            return (std::isalpha(c) || c == '_');
        }

        static bool IsNumeric(char c, bool allowsMinus)
        {
            return (std::isdigit(c) || (allowsMinus ? (c == '-') : false));
        }

        static bool IsAlphaNumeric(char c)
        {
            return IsAlpha(c) || IsNumeric(c, false);
        }
    }

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    Tokenizer::Tokenizer(std::string& fileContent)
        : m_FileContent(fileContent)
    {
    }

    std::vector<Token> Tokenizer::Tokenize()
    {
        m_Tokens.clear();
        m_Buffer.clear();
        m_LineNumber = 1;

        while (Peek(0).has_value())
        {
            // Is alphabetic
            if (PeekCheck(IsAlpha)) // Note: Also handles boolean values
            {
                m_Buffer.push_back(Consume());

                // While is alphabetic or a number, keep reading
                while (PeekCheck(IsAlphaNumeric))
                    m_Buffer.push_back(Consume());

                // If no type is found, try keywords.
                if (!HandleTypes())
                    HandleKeywords();
            }

            // Is number
            else if (PeekCheck(IsNumeric, true))
            {
                m_Buffer.push_back(Consume());

                // While still a numeric value, keep reading
                while (PeekCheck(IsNumeric, false))
                    m_Buffer.push_back(Consume());

                // If buffer is just a minus, make it a minus
                if (m_Buffer == "-")
                {
                    m_Tokens.emplace_back(TokenType::Minus, m_LineNumber);
                    m_Buffer.clear();
                    continue;
                }

                if (Peek(0).has_value() && (Peek(0).value() == '.'))
                {
                    m_Buffer.push_back(Consume()); // '.' char

                    while (PeekCheck(IsNumeric, false))
                        m_Buffer.push_back(Consume());

                    m_Tokens.emplace_back(TokenType::FloatLiteral, m_Buffer, m_LineNumber);
                    m_Buffer.clear();
                }
                else // Else it's just an integer literal
                {
                    m_Tokens.emplace_back(TokenType::IntegerLiteral, m_Buffer, m_LineNumber);
                    m_Buffer.clear();
                }
            }

            // Is char
            else if (Peek(0).has_value() && Peek(0).value() == '\'' // Begin char character
                && Peek(2).has_value() && Peek(2).value() == '\'') // End char character
            {
                Consume(); // ''' Start char character
                m_Buffer.push_back(Consume()); // Char character
                Consume(); // ''' End char character

                m_Tokens.emplace_back(TokenType::CharLiteral, m_Buffer, m_LineNumber);
                m_Buffer.clear();
            }

            // Is string // Note: String buffer keeps '\'s
            else if (Peek(0).has_value() && Peek(0).value() == '"')
            {
                Consume(); // '"' Start string character

                bool contin = Peek(0).has_value() && Peek(0).value() != '"';
                while (contin)
                {
                    m_Buffer.push_back(Consume());

                    if (Peek(0).has_value() && Peek(0).value() == '"' && Peek(-1).value() != '\\')
                        contin = false;
                }

                Consume(); // '"' End string character

                m_Tokens.emplace_back(TokenType::StringLiteral, m_Buffer, m_LineNumber);
                m_Buffer.clear();
            }

            // Single line comment
            else if (Peek(0).value() == '/' && Peek(1).has_value() && Peek(1).value() == '/') 
            {
                Consume(); // '/' char
                Consume(); // '/' char

                while (Peek(0).has_value() && (Peek(0).value() != '\n' && Peek(0).value() != '\r'))
                    Consume();

                Consume(); // '\n' char
                m_LineNumber++;
            }
            // Multiline comment
            else if (Peek(0).value() == '/' && Peek(1).has_value() && Peek(1).value() == '*')
            {
                Consume(); // '/' char
                Consume(); // '*' char

                while (Peek(0).has_value()) 
                {
                    if (Peek(0).value() == '*' && Peek(1).has_value() && Peek(1).value() == '/')
                    {
                        Consume(); // '*' char
                        Consume(); // '/' char
                        break;
                    }
                    else if ((Peek(0).value() == '\n' || Peek(0).value() == '\r'))
                        m_LineNumber++;

                    Consume();
                }
            }

            // Handle operators and other chars
            else if (HandleOperators())
                continue;

            // Invalid token
            else
            {
                CompilerSuite::Error(m_LineNumber, "Invalid token found: {0}", Peek(0).value());

                // Consume() just 1 char, just to make sure we keep going.
                // Since obviously from the previous char it was impossible to carry on.
                Consume();
                m_Buffer.clear();
            }
        }

        return m_Tokens;
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
    bool Tokenizer::HandleTypes()
    {
        // Note: It's okay to use if instead of else if, since
        // we return from the function if it has been found.
        #define HandleType(tokenType)                                               \
            if (m_Buffer == ValueTypeToStr(static_cast<ValueType>(tokenType)))      \
            {                                                                       \
                m_Tokens.emplace_back(tokenType, m_LineNumber);                     \
                m_Buffer.clear();                                                   \
                return true;                                                        \
            }

        HandleType(TokenType::Void);

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

    void Tokenizer::HandleKeywords()
    {
        // If / Else
        if (m_Buffer == "if")
        {
            m_Tokens.emplace_back(TokenType::If, m_LineNumber);
            m_Buffer.clear();
            return;
        }
        if (m_Buffer == "else")
        {
            m_Tokens.emplace_back(TokenType::Else, m_LineNumber);
            m_Buffer.clear();
            return;
        }

        // Boolean values
        if (m_Buffer == "false")
        {
            m_Tokens.emplace_back(TokenType::BoolLiteral, "0", m_LineNumber);
            m_Buffer.clear();
            return;
        }
        if (m_Buffer == "true")
        {
            m_Tokens.emplace_back(TokenType::BoolLiteral, "1", m_LineNumber);
            m_Buffer.clear();
            return;
        }

        // Other
        if (m_Buffer == "return")
        {
            m_Tokens.emplace_back(TokenType::Return, m_LineNumber);
            m_Buffer.clear();
            return;
        }

        // Exit function // TODO: Make reusable for all functions
        if (m_Buffer == "exit")
        {
            m_Tokens.emplace_back(TokenType::Exit, m_LineNumber);
            m_Buffer.clear();
            return;
        }

        // Else we say its an Identifier
        m_Tokens.emplace_back(TokenType::Identifier, m_Buffer, m_LineNumber);
        m_Buffer.clear();
    }

    bool Tokenizer::HandleOperators()
    {
        // Note: It's okay to use if instead of else if, since
        // we return from the function if it has been found.
        #define CharOperator(c, tokenType)                      \
            if (Peek().value() == c)                            \
            {                                                   \
                Consume();                                      \
                m_Tokens.emplace_back(tokenType, m_LineNumber); \
                return true;                                    \
            }

        // Signs
        CharOperator(';', TokenType::Semicolon);
        CharOperator('(', TokenType::OpenParenthesis);
        CharOperator(')', TokenType::CloseParenthesis);
        CharOperator('{', TokenType::OpenCurlyBrace);
        CharOperator('}', TokenType::CloseCurlyBrace);
        CharOperator('[', TokenType::OpenSquareBracket);
        CharOperator(']', TokenType::CloseSquareBracket);
        CharOperator('=', TokenType::Equals);

        CharOperator('.', TokenType::Period);
        CharOperator(',', TokenType::Comma);

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
            m_LineNumber++;
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

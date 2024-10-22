#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

namespace Dynamite
{

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    Tokenizer::Tokenizer(std::string& fileContent)
        : m_FileContent(fileContent)
    {
    }

    std::vector<Token> Tokenizer::Tokenize()
    {
        std::vector<Token> tokens = { };
        std::string buffer = {};
        uint32_t lineNumber = 1;

        while (Peek().HasValue())
        {
            // Is alphabetic
            if (PeekCheck(Utils::IsAlpha, std::initializer_list({ '_' }))) // Note: Also handles boolean values
            {
                buffer.push_back(Consume());

                // While is alphabetic or a number, keep reading
                while (PeekCheck(Utils::IsAlphaNumeric, std::initializer_list({ '_' })))
                    buffer.push_back(Consume());

                // If no type is found, try keywords.
                if (!HandleTypes(tokens, buffer, lineNumber))
                    HandleKeywords(tokens, buffer, lineNumber);

                continue;
            }

            // Is number
            else if (PeekCheck(Utils::IsNumeric, std::initializer_list({ '-' })))
            {
                buffer.push_back(Consume());

                // While still a numeric value, keep reading (also allows .'s for floats)
                while (PeekCheck(Utils::IsNumeric, std::initializer_list({ '.' })))
                    buffer.push_back(Consume());

                // If buffer is just a minus, make it a minus
                // this is necessary since we allow a '-' at the start
                // of a number.
                if (buffer == "-")
                {
                    tokens.emplace_back(TokenType::Minus, lineNumber);
                    buffer.clear();
                }
                else if (buffer.find('.') != std::string::npos)
                {
                    tokens.emplace_back(TokenType::FloatLiteral, buffer, lineNumber);
                    buffer.clear();
                }
                else // Else it's just an integer literal
                {
                    tokens.emplace_back(TokenType::IntegerLiteral, buffer, lineNumber);
                    buffer.clear();
                }

                continue;
            }

            // Character
            else if (PeekIs(0, '\'') && PeekIs(2, '\''))
            {
                Consume();                      // ''' Start char character
                buffer.push_back(Consume());    // Char character
                Consume();                      // ''' End char character

                tokens.emplace_back(TokenType::CharLiteral, buffer, lineNumber);
                buffer.clear();

                continue;
            }

            // String // Note: String buffer keeps \ 
            else if (PeekIs(0, '"'))
            {
                Consume(); // '"' Start string character

                bool contin = Peek(0).HasValue() && Peek(0).Value() != '"';
                while (contin)
                {
                    buffer.push_back(Consume());

                    if (PeekIs(0, '"') && !PeekIs(-1, '\\'))
                        contin = false;
                }

                Consume(); // '"' End string character

                tokens.emplace_back(TokenType::StringLiteral, buffer, lineNumber);
                buffer.clear();

                continue;
            }

            // Single line comment
            else if (PeekIs(0, '/') && PeekIs(1, '/'))
            {
                Consume(); // '/' char
                Consume(); // '/' char

                while (Peek(0).HasValue() && (Peek(0).Value() != '\n' && Peek(0).Value() != '\r'))
                    Consume();

                Consume(); // '\n' char
                lineNumber++;

                continue;
            }
            // Multiline comment
            else if (PeekIs(0, '/') && PeekIs(1, '*'))
            {
                Consume(); // '/' char
                Consume(); // '*' char

                while (Peek(0).HasValue())
                {
                    if (PeekIs(0, '*') && PeekIs(1, '/'))
                    {
                        Consume(); // '*' char
                        Consume(); // '/' char
                        break;
                    }
                    else if ((Peek(0).Value() == '\n' || Peek(0).Value() == '\r'))
                        lineNumber++;

                    Consume();
                }

                continue;
            }

            // Handle operators and other chars
            else if (HandleOperators(tokens, buffer, lineNumber))
                continue;

            // Invalid token
            else
            {
                Compiler::Error(lineNumber, "Invalid token found: {0}", Peek(0).Value());

                // Consume() just 1 char, just to make sure we keep going.
                // Since obviously from the previous char it was impossible to carry on.
                Consume();
                buffer.clear();
            }
        }

        return tokens;
    }

    /////////////////////////////////////////////////////////////////
    // Peeking & consuming
    /////////////////////////////////////////////////////////////////
    Optional<char> Tokenizer::Peek(size_t offset) const
    {
        if (m_Index + offset >= m_FileContent.size())
            return {};

        return m_FileContent.at(m_Index + offset);
    }

    char Tokenizer::Consume()
    {
        return m_FileContent.at(m_Index++);
    }

    bool Tokenizer::PeekIs(size_t offset, char c) const
    {
        auto value = Peek(offset);
        return (value.HasValue() && (value.Value() == c));
    }

    /////////////////////////////////////////////////////////////////
    // Handling functions
    /////////////////////////////////////////////////////////////////
    bool Tokenizer::HandleTypes(std::vector<Token>& tokens, std::string& buffer, uint32_t& line)
    {
        auto handleType = [&](TokenType type) -> bool
        {
            if (buffer == TokenTypeToString(type))
            {                                                                       
                tokens.emplace_back(type, line);
                buffer.clear();                                                   
                return true;                                                        
            }

            return false;
        };

        if (handleType(TokenType::Void))            return true;

        else if (handleType(TokenType::Bool))       return true;

        else if (handleType(TokenType::Int8))       return true;
        else if (handleType(TokenType::Int16))      return true;
        else if (handleType(TokenType::Int32))      return true;
        else if (handleType(TokenType::Int64))      return true;

        else if (handleType(TokenType::UInt8))      return true;
        else if (handleType(TokenType::UInt16))     return true;
        else if (handleType(TokenType::UInt32))     return true;
        else if (handleType(TokenType::UInt64))     return true;

        else if (handleType(TokenType::Float32))    return true;
        else if (handleType(TokenType::Float64))    return true;

        else if (handleType(TokenType::Char))       return true;
        else if (handleType(TokenType::String))     return true;

        return false;
    }

    bool Tokenizer::HandleKeywords(std::vector<Token>& tokens, std::string& buffer, uint32_t& line)
    {
        auto handleWord = [&](TokenType type, std::string compare = {}, const std::string & value = {}) -> bool
        {
            if (compare.empty())
                compare = TokenTypeToString(type);

            if (buffer == TokenTypeToString(type))
            {
                if (value.empty())
                    tokens.emplace_back(type, line);
                else
                    tokens.emplace_back(type, value, line);

                buffer.clear();
                return true;
            }

            return false;
        };

        // If / Else
        if (handleWord(TokenType::If))                              return true;
        else if (handleWord(TokenType::Else))                       return true;

        // Boolean values
        else if (handleWord(TokenType::BoolLiteral, "false", "0"))  return true;
        else if (handleWord(TokenType::BoolLiteral, "true", "1"))   return true;

        // Specifiers
        else if (handleWord(TokenType::Const))                      return true;
        else if (handleWord(TokenType::Volatile))                   return true;

        // Other
        else if (handleWord(TokenType::Return))                     return true;

        // Else we say its an Identifier
        tokens.emplace_back(TokenType::Identifier, buffer, line);
        buffer.clear();
        return false;
    }

    bool Tokenizer::HandleOperators(std::vector<Token>& tokens, std::string& buffer, uint32_t& line)
    {
        auto handleOperator = [&](TokenType type) -> bool
        {
            if (std::string(1, Peek(0).Value()) == TokenTypeToString(type))
            {
                Consume();
                tokens.emplace_back(type, line);
                return true;
            }

            return false;
        };

        // Signs
        if (handleOperator(TokenType::Semicolon))               return true;

        else if (handleOperator(TokenType::OpenParenthesis))    return true;
        else if (handleOperator(TokenType::CloseParenthesis))   return true;
        else if (handleOperator(TokenType::OpenCurlyBrace))     return true;
        else if (handleOperator(TokenType::CloseCurlyBrace))    return true;
        else if (handleOperator(TokenType::OpenSquareBracket))  return true;
        else if (handleOperator(TokenType::CloseSquareBracket)) return true;

        else if (handleOperator(TokenType::Equals))             return true;
        else if (handleOperator(TokenType::Colon))              return true;

        else if (handleOperator(TokenType::Period))             return true;
        else if (handleOperator(TokenType::Comma))              return true;

        // Operators
        else if (handleOperator(TokenType::Plus))               return true;
        else if (handleOperator(TokenType::Minus))              return true;
        else if (handleOperator(TokenType::Star))               return true;
        else if (handleOperator(TokenType::Divide))             return true;

        else if (handleOperator(TokenType::Or))                 return true;
        else if (handleOperator(TokenType::And))                return true;
        else if (handleOperator(TokenType::Xor))                return true;

        // Newline (for incrementing)
        else if (PeekIs(0, '\n') || PeekIs(0, '\r'))
        {
            Consume();
            line++;
            return true;
        }
        // Space
        else if (PeekCheck(Utils::IsSpace, std::initializer_list<char>()))
        {
            Consume();
            return true;
        }

        return false;
    }

}
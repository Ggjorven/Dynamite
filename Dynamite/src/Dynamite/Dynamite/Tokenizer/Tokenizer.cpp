#include "dypch.h"
#include "Tokenizer.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"
#include "Dynamite/Utils/Optional.hpp"

#include <regex>

namespace Dynamite
{

    namespace
    {
        static Tokenizer* s_Instance = nullptr;
    }

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    Tokenizer::Tokenizer(std::string& fileContent)
        : m_FileContent(fileContent)
    {
        s_Instance = this;
    }

    Tokenizer::~Tokenizer()
    {
        s_Instance = nullptr;
    }

    std::vector<Token> Tokenizer::Tokenize()
    {
        std::vector<Token> tokens = { };
        std::string buffer = {};
        uint32_t lineNumber = 1;

        m_Index = 0;

        while (Peek().HasValue())
        {
            // Is alphabetic
            if (Utils::OptCheck(Peek(0), Utils::IsAlpha, std::vector({ '_' }))) // Note: Also handles boolean values
            {
                buffer.push_back(Consume());

                // While is alphabetic or a number, keep reading
                while (Utils::OptCheck(Peek(0), Utils::IsAlphaNumeric, std::vector({ '_' })))
                    buffer.push_back(Consume());

                // If no type is found, try keywords.
                if (!HandleTypes(tokens, buffer, lineNumber))
                    HandleKeywords(tokens, buffer, lineNumber);

                continue;
            }

            // Is number
            else if (Utils::OptCheck(Peek(0), Utils::IsNumeric, std::vector({ '-' })))
            {
                buffer.push_back(Consume());

                // While still a numeric value, keep reading (also allows .'s for floats)
                while (Utils::OptCheck(Peek(0), Utils::IsNumeric, std::vector({ '.' })))
                    buffer.push_back(Consume());

                // If buffer is just a minus, make it a minus
                // this is necessary since we allow a '-' at the start
                // of a number.
                if (buffer == "-")
                {
                    if (Peek(0).Value() == '>')
                    {
                        Consume();
                        tokens.emplace_back(TokenType::Arrow, lineNumber);
                    }
                    else
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
            else if (Utils::OptCheck(Peek(0), '\'') && Utils::OptCheck(Peek(2), '\''))
            {
                Consume();                      // ''' Start char character

                bool contin = Peek(0).HasValue() && Peek(0).Value() != '\'';
                while (contin)
                {
                    buffer.push_back(Consume());

                    if (Utils::OptCheck(Peek(0), '\'') && !Utils::OptCheck(Peek(-1), '\\'))
                        contin = false;
                }

                Consume();                      // ''' End char character

                HandleSpecialCharacters(buffer);

                if (buffer.size() > 1)
                    Compiler::Error(lineNumber, "Character literal cannot be more than one character.");

                tokens.emplace_back(TokenType::CharLiteral, buffer, lineNumber);
                buffer.clear();

                continue;
            }

            // Character array // Note: Buffer keeps \ 
            else if (Utils::OptCheck(Peek(0), '"'))
            {
                Consume(); // '"' Start array

                bool contin = Peek(0).HasValue() && Peek(0).Value() != '"';
                while (contin)
                {
                    buffer.push_back(Consume());

                    if (Utils::OptCheck(Peek(0), '"') && !Utils::OptCheck(Peek(-1), '\\'))
                        contin = false;
                }

                Consume(); // '"' End array

                HandleSpecialCharacters(buffer);
                
                tokens.emplace_back(TokenType::CharArrayLiteral, buffer, lineNumber);
                buffer.clear();

                continue;
            }

            // Single line comment
            else if (Utils::OptCheck(Peek(0), '/') && Utils::OptCheck(Peek(1), '/'))
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
            else if (Utils::OptCheck(Peek(0), '/') && Utils::OptCheck(Peek(1), '*'))
            {
                Consume(); // '/' char
                Consume(); // '*' char

                while (Peek(0).HasValue())
                {
                    if (Utils::OptCheck(Peek(0), '*') && Utils::OptCheck(Peek(1), '/'))
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
    Optional<char> Tokenizer::Peek(int64_t offset) const
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

        return false;
    }

    bool Tokenizer::HandleKeywords(std::vector<Token>& tokens, std::string& buffer, uint32_t& line)
    {
        auto handleWord = [&](TokenType type, std::string compare = {}, const std::string & value = {}) -> bool
        {
            if (compare.empty())
                compare = TokenTypeToString(type);

            if (buffer == compare)
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

        // Boolean values
        if (handleWord(TokenType::BoolLiteral, "false", "0"))       return true;
        else if (handleWord(TokenType::BoolLiteral, "true", "1"))   return true;

        // Specifiers
        else if (handleWord(TokenType::Mut))                        return true;
        else if (handleWord(TokenType::Volatile))                   return true;

        // Keywords
        else if (handleWord(TokenType::Struct))                     return true;
        else if (handleWord(TokenType::Class))                      return true;

        else if (handleWord(TokenType::Enum))                       return true;
        else if (handleWord(TokenType::Type))                       return true;

        else if (handleWord(TokenType::Let))                        return true;
        else if (handleWord(TokenType::Fn))                         return true;

        else if (handleWord(TokenType::If))                         return true;
        else if (handleWord(TokenType::Else))                       return true;

        else if (handleWord(TokenType::While))                      return true;
        else if (handleWord(TokenType::For))                        return true;

        else if (handleWord(TokenType::In))                         return true;

        else if (handleWord(TokenType::Return))                     return true;

        else if (handleWord(TokenType::Pub))                        return true;
        else if (handleWord(TokenType::Priv))                       return true;

        else if (handleWord(TokenType::As))                         return true;

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
        else if (Peek(0).Value() == '-' && Peek(1).Value() == '>')
        {
            Consume();
            tokens.emplace_back(TokenType::Arrow, line);
            return true;
        }

        else if (handleOperator(TokenType::Plus))               return true;
        else if (handleOperator(TokenType::Minus))              return true;
        else if (handleOperator(TokenType::Star))               return true;
        else if (handleOperator(TokenType::ForwardSlash))       return true;

        else if (handleOperator(TokenType::Pipe))               return true;
        else if (handleOperator(TokenType::And))                return true;
        else if (handleOperator(TokenType::UpArrow))            return true;

        else if (handleOperator(TokenType::TakeAddress))        return true;

        else if (handleOperator(TokenType::At))         return true;


        // Newline (for incrementing)
        else if (Utils::OptCheck(Peek(0), '\n') || Utils::OptCheck(Peek(0), '\r'))
        {
            Consume();
            line++;
            return true;
        }
        // Space
        else if (Utils::OptCheck(Peek(0), Utils::IsSpace, std::vector<char>()))
        {
            Consume();
            return true;
        }

        return false;
    }

    void Tokenizer::HandleSpecialCharacters(std::string& str)
    {
        std::string result;
        result.reserve(str.size());  // Reserve enough space to avoid multiple allocations.

        for (size_t i = 0; i < str.size(); i++) 
        {
            if (str[i] == '\\' && i + 1 < str.size()) 
            {
                switch (str[i + 1]) 
                {
                case 'n': 
                    result += '\n'; 
                    break;
                case 't': 
                    result += '\t'; 
                    break;
                case 'r': 
                    result += '\r'; 
                    break;
                case 'f':
                    result += '\f'; 
                    break;
                case 'v':
                    result += '\v'; 
                    break;
                case '\\': 
                    result += '\\'; 
                    break;
                case '"': 
                    result += '\"'; 
                    break;
                case '\'': 
                    result += '\''; 
                    break;

                default:
                    result += '\\';  // Keep the backslash if it's not a special character.
                    result += str[i + 1];
                    break;
                }

                ++i;  // Skip the next character as it's part of the escape sequence.
            }
            else 
            {
                result += str[i];
            }
        }

        str = std::move(result);  // Replace the original string with the result.
    }

}
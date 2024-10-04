#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite
{

    Generator::Generator(const Nodes::Program& program)
        : m_Program(program)
    {
    }

    void Generator::GenerateExpression(std::stringstream& output, const Nodes::Expression* expr)
    {
        switch (expr->ExpressionType)
        {
        case Nodes::Expression::Type::Int64Literal:
        {
            output << "\tmov rax, " << expr->Int64LiteralObj->TokenObj.Value.value() << "\n";
            Push(output, "rax", VariableTypeSize(GetVariableType(expr->ExpressionType)));
            break;
        }
        case Nodes::Expression::Type::Identifier:
        {
            if (!m_Variables.contains(expr->IdentifierObj->TokenObj.Value.value()))
            {
                DY_LOG_ERROR("Use of undeclared identifier '{0}', line: {1}", expr->IdentifierObj->TokenObj.Value.value(), expr->IdentifierObj->TokenObj.LineNumber);
                break;
            }
            
            const auto& var = m_Variables.at(expr->IdentifierObj->TokenObj.Value.value());
            Push(output, Pulse::Text::Format("{0} [rsp + {1}]", VariableTypeToASM(var.Type), (m_StackSize - var.StackLocation) - VariableTypeSize(var.Type)), VariableTypeSize(var.Type));
            break;
        }

        default:
            break;
        }
    }

    void Generator::GenerateStatement(std::stringstream& output, const Nodes::Statement* statement)
    {
        switch (statement->StatementType)
        {
        case Nodes::Statement::Type::Exit:
        {
            GenerateExpression(output, statement->ExitObj->ExpressionObj);
            output << "\tmov rax, 60\n";
            Pop(output, "rdi", VariableTypeSize(GetVariableType(Nodes::Expression::Type::Int64Literal)));
            output << "\tsyscall\n";
            break;
        }

        case Nodes::Statement::Type::Let:
        {
            if (m_Variables.contains(statement->LetObj->TokenObj.Value.value()))
            {
                DY_LOG_ERROR("Identifier already used: '{0}', line: {1}", statement->LetObj->TokenObj.Value.value(), statement->LetObj->TokenObj.LineNumber);
                break;
            }
            
            m_Variables.insert({ 
                statement->LetObj->TokenObj.Value.value(), 
                Variable { 
                    .Type = GetVariableType(statement->LetObj->ExpressionObj->IdentifierObj->TokenObj.Type),
                    .StackLocation = m_StackSize 
                } 
            });

            GenerateExpression(output, statement->LetObj->ExpressionObj);
            break;
        }

        default:
            break;
        }
    }

    std::string Generator::Generate()
    {
        std::stringstream output = {};
        output << "global _start\n_start:\n";

        for (const Nodes::Statement* stmt : m_Program.Statements)
            GenerateStatement(output, stmt);

        // Default exit code of 0
        output << "\tmov rax, 60\n";
        output << "\tmov rdi, 0\n";
        output << "\tsyscall\n";

        return output.str();
    }

    void Generator::Push(std::stringstream& output, const std::string& reg, size_t size)
    {
        output << "\tpush " << reg << "\n";
        m_StackSize += size;
    }

    void Generator::Pop(std::stringstream& output, const std::string& reg, size_t size)
    {
        output << "\tpop " << reg << "\n";
        m_StackSize -= size;
    }



    VariableType GetVariableType(TokenType tokenType)
    {
        switch (tokenType)
        {
        case TokenType::Int64Literal:                   return VariableType::Int64;

        default:
            break;
        }

        DY_LOG_ERROR("TokenType::{0}, VariableType has not been defined.", Pulse::Enum::Name(tokenType));
        return VariableType::None;
    }

    VariableType GetVariableType(Nodes::Expression::Type exprType)
    {
        switch (exprType)
        {
        case Nodes::Expression::Type::Int64Literal:     return VariableType::Int64;

        default:
            break;
        }

        DY_LOG_ERROR("Expression::Type::{0}, VariableType has not been defined.", Pulse::Enum::Name(exprType));
        return VariableType::None;
    }

    size_t VariableTypeSize(VariableType type)
    {
        switch (type)
        {
        case VariableType::Int64:     return sizeof(int64_t);
        
        default:
            break;
        }

        DY_LOG_ERROR("VariableType::{0}, size has not been defined.", Pulse::Enum::Name(type));
        return 0;
    }

    std::string VariableTypeToASM(VariableType type)
    {
        switch (type)
        {
        case VariableType::Int64:     return "QWORD";

        default:
            break;
        }

        DY_LOG_ERROR("VariableType::{0}, ASM type has not been defined.", Pulse::Enum::Name(type));
        return "Undefined ASM Type";
    }

}
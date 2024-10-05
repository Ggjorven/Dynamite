#include "dypch.h"
#include "Generator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

namespace Dynamite
{

    Generator::Generator(const Nodes::Program& program)
        : m_Program(program)
    {
    }

    void Generator::GenerateTerm(std::stringstream& output, const Nodes::Expression::Term* term)
    {
        switch (term->TermType)
        {
        case Nodes::Expression::Term::Type::Int64Literal:
        {
            output << "\tmov rax, " << term->Int64LiteralObj->TokenObj.Value.value() << "\n";
            Push(output, "rax", VariableTypeSize(GetVariableType(term->TermType)));
            break;
        }
        case Nodes::Expression::Term::Type::Identifier:
        {
            if (!m_Variables.contains(term->IdentifierObj->TokenObj.Value.value()))
            {
                DY_LOG_ERROR("Use of undeclared identifier '{0}', line: {1}", term->IdentifierObj->TokenObj.Value.value(), term->IdentifierObj->TokenObj.LineNumber);
                break;
            }

            const auto& var = m_Variables.at(term->IdentifierObj->TokenObj.Value.value());
            Push(output, Pulse::Text::Format("{0} [rsp + {1}]", VariableTypeToASM(var.Type), (m_StackSize - var.StackLocation) - VariableTypeSize(var.Type)), VariableTypeSize(var.Type));
            break;
        }

        default:
            break;
        }
    }

    void Generator::GenerateBinary(std::stringstream& output, const Nodes::Expression::Binary* binary)
    {
        switch (binary->BinaryType)
        {
        case Nodes::Expression::Binary::Type::Addition:
        {
            GenerateExpression(output, binary->AdditionObj->LHS);
            GenerateExpression(output, binary->AdditionObj->RHS);

            Pop(output, "rax", Nodes::VariableTypeSize(binary->AdditionObj->LHS->ResultType)); 
            Pop(output, "rbx", Nodes::VariableTypeSize(binary->AdditionObj->RHS->ResultType)); 
            
            output << "    add rax, rbx\n";
            
            // Push result on to the stack
            Push(output, "rax", Nodes::VariableTypeSize(binary->AdditionObj->LHS->ResultType));
            break;
        }
        case Nodes::Expression::Binary::Type::Multiply:
        {
            // TODO: ...
            break;
        }

        default:
            break;
        }
    }

    // Note to self: Adds expression/variable to stack temporarily or permanently
    void Generator::GenerateExpression(std::stringstream& output, const Nodes::Expression* expr)
    {
        switch (expr->ExpressionType)
        {
        case Nodes::Expression::Type::Term:
        {
            GenerateTerm(output, expr->TermObj);
            break;
        }
        case Nodes::Expression::Type::Binary:
        {
            GenerateBinary(output, expr->BinaryObj);
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
            Pop(output, "rdi", Nodes::VariableTypeSize(statement->ExitObj->ExpressionObj->ResultType));
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
                    .Type = statement->LetObj->ExpressionObj->ResultType,
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

}
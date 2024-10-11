#include "dypch.h"
#include "ASMGenerator.hpp"

#include "Dynamite/Core/Logging.hpp"

#include <Pulse/Text/Format.hpp>

namespace Dynamite
{

	void ASMGenerator::Generate(Node::Program& program, const std::filesystem::path& outputPath)
	{
		m_OutputPath = outputPath;
		m_Output.clear();

		std::filesystem::path dir = outputPath.parent_path();
		std::filesystem::path result = dir / std::filesystem::path(outputPath.filename()).replace_extension(".asm");

		std::ofstream output(result);
		output << GenProgram();
	}

	void ASMGenerator::GenTermExpr(const Node::Reference<Node::TermExpr> term)
	{
		struct TermVisitor 
		{
		public:
			ASMGenerator& Generator;

		public:
			void operator() (const Node::Reference<Node::LiteralTerm> literalTerm) const
			{
				ValueType valueType = GetValueType(static_cast<TokenType>(literalTerm->LiteralType), literalTerm->TokenObj.Value.value());

				switch (literalTerm->LiteralType)
				{
				case Node::LiteralTerm::Type::Integer:
				{
					// TODO: ...
					break;
				}
				// TODO: Other types

				default:
					break;
				}
			}
			void operator() (const Node::Reference<Node::IdentifierTerm> identifier) {}
			void operator() (const Node::Reference<Node::ParenthesisTerm> parenthesis) {}
		};

		TermVisitor visitor = { .Generator = *this };
		std::visit(visitor, term->TermObj);
	}

	void ASMGenerator::GenBinaryExpr(const Node::Reference<Node::BinaryExpr> bin_expr)
	{
	}

	void ASMGenerator::GenExpr(const Node::Reference<Node::Expression> expr)
	{
	}

	void ASMGenerator::GenScope(const Node::Reference<Node::ScopeStatement> scope)
	{
	}

	void ASMGenerator::GenConditionBranch(const Node::Reference<Node::ConditionBranch> branch, const std::string& endLabel)
	{
	}

	void ASMGenerator::GenStatement(const Node::Reference<Node::Statement> statement)
	{
	}

	std::string ASMGenerator::GenProgram()
	{
		Node::Program program = {};

		m_Output << "global _start\n";
		m_Output << "_start:\n";

		for (const auto& statement : program.Statements)
			GenStatement(statement);

		// 60 is the syscall for exit on linux.
		// Default exit code of 0
		m_Output << "    mov rax, 60\n";
		m_Output << "    mov rdi, 0\n";
		m_Output << "    syscall\n";

		return m_Output.str();
	}

	void ASMGenerator::Push(ValueType type, const std::string& reg)
	{
	}

	void ASMGenerator::Pop(ValueType type, const std::string& reg)
	{
	}

	void ASMGenerator::BeginScope()
	{
		m_Scopes.push_back(m_Variables.size());
	}

	void ASMGenerator::EndScope()
	{
		const size_t popCount = m_Variables.size() - m_Scopes.back();
		size_t removeSize = 0;

		if (popCount != 0)
		{
			for (size_t i = (m_Variables.size() - 1); i > ((m_Variables.size() - 1) - popCount); i--)
				removeSize += ValueTypeSize(m_Variables[i].Type);

			m_Output << "\tadd rsp, " << removeSize << "\n";
		}
		
		m_StackSize -= removeSize;
		for (size_t i = 0; i < popCount; i++)
			m_Variables.pop_back();

		m_Scopes.pop_back();
	}

	std::string ASMGenerator::CreateLabel()
	{
		return Pulse::Text::Format("_Dynamite_Label_{0}", m_LabelCount++);
	}

}
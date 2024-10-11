#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Nodes.hpp"
#include "Dynamite/Parsing/Variables.hpp"

#include "Dynamite/Generator/Generator.hpp"

#include <filesystem>

namespace Dynamite
{

	struct ASMVariable
	{
	public:
		std::string Name = {};
		ValueType Type = ValueType::None;

		size_t StackLocation = 0;
	};

	// Note: We use the linux calling convention.
	class ASMGenerator : public Generator
	{
	public:
		ASMGenerator() = default;
		virtual ~ASMGenerator() = default;

		// Note: The extension of the outputPath is irrelevant
		void Generate(Node::Program& program, const std::filesystem::path& outputPath) override;

	public:
		void GenTermExpr(const Node::Reference<Node::TermExpr> term);
		void GenBinaryExpr(const Node::Reference<Node::BinaryExpr> bin_expr);
		void GenExpr(const Node::Reference<Node::Expression> expr);

		void GenScope(const Node::Reference<Node::ScopeStatement> scope);
		void GenConditionBranch(const Node::Reference<Node::ConditionBranch> branch, const std::string& endLabel);
		void GenStatement(const Node::Reference<Node::Statement> statement);

		[[nodiscard]] std::string GenProgram();

		void Push(ValueType type, const std::string& reg);
		void Pop(ValueType type, const std::string& reg);

		void BeginScope();
		void EndScope();

		std::string CreateLabel();

	private:
		std::stringstream m_Output = {};
		std::filesystem::path m_OutputPath = {};

		size_t m_StackSize = 0;

		std::vector<ASMVariable> m_Variables = { };
		std::vector<size_t> m_Scopes = { 0 };
		
		size_t m_LabelCount = 0;
	};

}
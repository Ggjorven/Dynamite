#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Nodes.hpp"

namespace Dynamite
{

	class Generator
	{
	public:
		explicit Generator(const Nodes::Program& program);
		~Generator() = default;

		void GenerateTerm(std::stringstream& output, const Nodes::Expression::Term* term);
		void GenerateBinary(std::stringstream& output, const Nodes::Expression::Binary* binary);

		void GenerateExpression(std::stringstream& output, const Nodes::Expression* expr);
		void GenerateStatement(std::stringstream& output, const Nodes::Statement* statement);

		// Generates the full program assembly
		[[nodiscard]] std::string Generate();

	private:
		void Push(std::stringstream& output, const std::string& reg, size_t size);
		void Pop(std::stringstream& output, const std::string& reg, size_t size);

	private:
		struct Variable
		{
		public:
			Nodes::VariableType Type = Nodes::VariableType::None;
			size_t StackLocation = 0;
		};

	private:
		const Nodes::Program& m_Program;
		
		size_t m_StackSize = 0;
		std::unordered_map<std::string, Variable> m_Variables = { };
	};

}
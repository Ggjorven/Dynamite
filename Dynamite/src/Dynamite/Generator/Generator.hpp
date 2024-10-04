#pragma once

#include "Dynamite/Tokens/Token.hpp"

#include "Dynamite/Parsing/Nodes.hpp"

namespace Dynamite
{

	enum class VariableType : uint8_t
	{
		None = 0, 
		Int64,
	};
	VariableType GetVariableType(TokenType tokenType);
	VariableType GetVariableType(Nodes::Expression::Type exprType);
	size_t VariableTypeSize(VariableType type);
	std::string VariableTypeToASM(VariableType type);

	class Generator
	{
	public:
		explicit Generator(const Nodes::Program& program);
		~Generator() = default;

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
			VariableType Type = VariableType::None;
			size_t StackLocation = 0;
		};

	private:
		const Nodes::Program& m_Program;
		
		size_t m_StackSize = 0;
		std::unordered_map<std::string, Variable> m_Variables = { };
	};

}
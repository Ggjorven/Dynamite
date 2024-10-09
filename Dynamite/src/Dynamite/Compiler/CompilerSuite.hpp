#pragma once

#include "Dynamite/Tokens/Tokenizer.hpp"
#include "Dynamite/Parsing/Parser.hpp"
#include "Dynamite/Generator/Generator.hpp"

#include "Dynamite/Compiler/CompilerOptions.hpp"

#include <Pulse/Core/Unique.hpp>

#include <cstdint>
#include <string>
#include <optional>

namespace Dynamite
{

	class CompilerSuite
	{
	public:
		CompilerSuite(const CompilerOptions& options);
		~CompilerSuite();

		std::string Compile();

		inline const Tokenizer* GetTokenizer() const { return m_Tokenizer.Raw(); }
		inline const Parser* GetParser() const { return m_Parser.Raw(); }

	private:
		const CompilerOptions m_Options;

		Pulse::Unique<Tokenizer> m_Tokenizer = nullptr;
		Pulse::Unique<Parser> m_Parser = nullptr;
		//Pulse::Unique<Generator> m_Generator = nullptr;
	};

}
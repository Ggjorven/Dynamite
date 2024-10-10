#include "dypch.h"
#include "CompilerSuite.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite
{

	namespace
	{
		static CompilerSuite* s_Instance = nullptr;
	}

	CompilerSuite::CompilerSuite(const CompilerOptions& options)
		: m_Options(options)
	{
		s_Instance = this;

		m_Tokenizer = Pulse::Unique<Tokenizer>::Create(m_CurrentFileContent);
		m_Parser = Pulse::Unique<Parser>::Create(m_CurrentTokens);
	}

	CompilerSuite::~CompilerSuite()
	{
		s_Instance = nullptr;
	}

	void CompilerSuite::Compile()
	{
		// Note: IncludeDirs are currently not used.
		const std::vector<std::string> files = m_Options.Get(CompilerFlag::Type::File);
		if (files.size() == 0)
		{
			DY_LOG_ERROR("Compilation terminated, no files specified.");
			return;
		}

		const std::string outputDir = m_Options.Get(CompilerFlag::Type::OutputDir).back();
		for (const auto& file : files)
		{
			DY_LOG_TRACE("Compiling '{0}'.", file);
			m_CurrentFile = file;

			std::ifstream input(file);
			
			std::stringstream contentBuffer;
			contentBuffer << input.rdbuf();
			m_CurrentFileContent = contentBuffer.str();

			// Note: Because the tokenizer and parser keep references to
			// member variables we don't need to pass in anything.
			m_CurrentState = State::Tokenizing;
			m_CurrentTokens = m_Tokenizer->Tokenize();

			m_CurrentState = State::Parsing;
			m_CurrentProgram = m_Parser->GetProgram();

			m_CurrentState = State::Generating;
			// TODO: Generate something with the program


			// Log extra info when verbosity is enabled
			if (m_Options.Contains(CompilerFlag::Type::Verbose))
			{
				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Tokens generated.");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& token : m_CurrentTokens)
					DY_LOG_TRACE(FormatToken(token));

				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Tree generated.");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& statement : m_CurrentProgram.Statements)
					DY_LOG_TRACE(Node::FormatStatementData(statement));
			}
		}
	}

	CompilerSuite& CompilerSuite::Get()
	{
		return *s_Instance;
	}

	const std::string CompilerSuite::GetCurrentLine(uint32_t line) const
	{
		// Find the start of the line by counting newlines
		size_t currentLine = 1;
		size_t start = 0;

		while (currentLine < line && start != std::string::npos)
		{
			start = m_CurrentFileContent.find_first_of("\n\r", start + 1);
			currentLine++;
		}

		// If the line was not found, return an empty string
		if (start == std::string::npos)
			return "";

		// Move to the start of the next line
		start++;

		// Find the end of the line
		size_t end = m_CurrentFileContent.find_first_of("\n\r", start);
		if (end == std::string::npos)
			end = m_CurrentFileContent.size();

		return m_CurrentFileContent.substr(start, end - start);
	}

}
#include "dypch.h"
#include "Compiler.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite
{

	namespace
	{
		static Compiler* s_Instance = nullptr;
	}

	Compiler::Compiler(const CompilerOptions& options)
		: m_Options(options)
	{
		s_Instance = this;
	}

	Compiler::~Compiler()
	{
		s_Instance = nullptr;
	}

	void Compiler::Compile()
	{
		// Note: Break when no files are specified.
		if (m_Options.Files.empty())
		{
			DY_LOG_ERROR("Compilation terminated, no files specified.");
			return;
		}

		Tokenizer tokenizer(m_FileContent);
		// ...
		
		for (const auto& file : m_Options.Files)
		{
			m_File = file;
			DY_LOG_TRACE("Compiling '{0}'.", m_File.string());
			
			// Read contents
			std::ifstream input(m_File);

			std::stringstream contentBuffer;
			contentBuffer << input.rdbuf();
			m_FileContent = contentBuffer.str();

			// Use contents
			m_State = State::Tokenizing;
			m_Tokens = tokenizer.Tokenize();

			// Log extra info when verbosity is enabled
			if (m_Options.Contains(CompilerFlag::Verbose))
			{
				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Tokens generated.");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& token : m_Tokens)
					DY_LOG_TRACE(TokenToString(token));
			}
		}
	}

	Compiler& Compiler::Get()
	{
		return *s_Instance;
	}

	const std::string Compiler::GetCurrentLine(uint32_t line) const
	{
		// Find the start of the line by counting newlines
		size_t currentLine = 1;
		size_t start = 0;

		while (currentLine < line && start != std::string::npos)
		{
			start = m_FileContent.find_first_of("\n\r", start + 1);
			currentLine++;
		}

		// If the line was not found, return an empty string
		if (start == std::string::npos)
			return "";

		// Move to the start of the next line
		start++;

		// Find the end of the line
		size_t end = m_FileContent.find_first_of("\n\r", start);
		if (end == std::string::npos)
			end = m_FileContent.size();

		return m_FileContent.substr(start, end - start);
	}

}
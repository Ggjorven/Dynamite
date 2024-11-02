#include "dypch.h"
#include "Compiler.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Dynamite/Tokenizer/Tokenizer.hpp"
#include "Dynamite/Dynamite/Parser/Parser.hpp"

#include "Dynamite/Linker/Linker.hpp"

namespace Dynamite
{

	namespace
	{
		static Compiler* s_Instance = nullptr;
	}

	/////////////////////////////////////////////////////////////////
	// Main functions
	/////////////////////////////////////////////////////////////////
	Compiler::Compiler(const CompilerOptions& options)
		: m_State(State::Preprocessing), m_Options(options)
	{
		s_Instance = this;
	}

	Compiler::~Compiler()
	{
		s_Instance = nullptr;
	}

	void Compiler::Compile()
	{
		if (m_Options.Contains(CompilerFlag::Help))
			return;

		// Note: Break when no files are specified.
		else if (m_Options.Files.empty())
		{
			DY_LOG_ERROR("Compilation terminated, no files specified.");
			return;
		}

		Tokenizer tokenizer(m_FileContent);
		Parser parser(m_Tokens);
		Generator generator(m_Program, m_GeneratedIR);
		
		auto startTime = std::chrono::steady_clock::now();

		for (const auto& file : m_Options.Files)
		{
			m_CurrentFile = file;
			DY_LOG_INFO("Compiling '{0}'.", m_CurrentFile.string());
			
			// Read contents
			std::ifstream input(m_CurrentFile);

			std::stringstream contentBuffer;
			contentBuffer << input.rdbuf();
			m_FileContent = contentBuffer.str();

			// Use contents
			if (m_ErrorCount == 0)
			{
				m_State = State::Tokenizing;
				m_Tokens = tokenizer.Tokenize();
			}
			if (m_ErrorCount == 0)
			{
				m_State = State::Parsing;
				m_Program = parser.GetProgram();
			}
			if (m_ErrorCount == 0)
			{
				m_State = State::Generating;
				generator.Generate(m_Options.OutputDir / file.filename());
			}

			// Log extra info when verbosity is enabled
			if (m_Options.Contains(CompilerFlag::Verbose))
			{
				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Generated tokens");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& token : m_Tokens)
					DY_LOG_TRACE("{0}", TokenToString(token));
				
				DY_LOG_TRACE("");

				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Generated nodes");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& definition : m_Program.Definitions)
				{
					DY_LOG_TRACE("{0}", DefineToString(definition));
					DY_LOG_TRACE("");
				}

				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Generated IR");
				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("{0}", m_GeneratedIR);

			}
		}

		auto endTime = std::chrono::steady_clock::now();
		DY_LOG_TRACE("Compilation took {0}ms.", std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

		if (m_ErrorCount == 0)
		{
			std::vector<std::filesystem::path> objectPaths = m_Options.Files;
			for (auto& path : objectPaths)
				path = m_Options.OutputDir / CompilerOptions::IntermediateDirectory / path.filename().replace_extension(".o");

			m_State = State::Linking;
			Linker::Link(m_Options.OutputDir, objectPaths);
			
			DY_LOG_TRACE("Linking took {0}ms.", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - endTime).count());
		}
	}

	Compiler& Compiler::Get()
	{
		return *s_Instance;
	}

	/////////////////////////////////////////////////////////////////
	// Private methods
	/////////////////////////////////////////////////////////////////
	std::string Compiler::GetCurrentLine(uint32_t line) const
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
		if (line != 1)
			start++;

		// Find the end of the line
		size_t end = m_FileContent.find_first_of("\n\r", start);
		if (end == std::string::npos)
			end = m_FileContent.size();

		std::string lineStr = m_FileContent.substr(start, end - start);
		std::string result = Utils::RemoveFrontIndentation(lineStr);
		return result;
	}

}
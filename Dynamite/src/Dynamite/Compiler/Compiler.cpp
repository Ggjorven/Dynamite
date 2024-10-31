#include "dypch.h"
#include "Compiler.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"

#include "Dynamite/Generator/Main/Linker.hpp"

#include <chrono>

namespace Dynamite
{

	namespace
	{
		static Compiler* s_Instance = nullptr;
	}

	Compiler::Compiler(const CompilerOptions& options)
		: m_State(State::Tokenizing), m_Options(options)
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
		Generator generator(m_Program);
		
		auto startTime = std::chrono::steady_clock::now();

		for (const auto& file : m_Options.Files)
		{
			m_File = file;
			DY_LOG_INFO("Compiling '{0}'.", m_File.string());
			
			// Read contents
			std::ifstream input(m_File);

			std::stringstream contentBuffer;
			contentBuffer << input.rdbuf();
			m_FileContent = contentBuffer.str();

			// Use contents
			m_State = State::Tokenizing;
			m_Tokens = tokenizer.Tokenize();

			m_State = State::Parsing;
			m_Program = parser.GetProgram();

			m_State = State::Generating;
			generator.Generate(m_Options.OutputDir / file.filename());

			// Log extra info when verbosity is enabled
			if (m_Options.Contains(CompilerFlag::Verbose))
			{
				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Tokens generated.");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& token : m_Tokens)
					DY_LOG_TRACE("{0}", TokenToString(token));
				
				DY_LOG_TRACE("");

				DY_LOG_TRACE("---------------------------------------");
				DY_LOG_TRACE("-- Tree generated.");
				DY_LOG_TRACE("---------------------------------------");

				for (const auto& definition : m_Program.Definitions)
				{
					DY_LOG_TRACE("{0}", DefineToString(definition));
					DY_LOG_TRACE("");
				}
			}
		}

		auto endTime = std::chrono::steady_clock::now();
		DY_LOG_TRACE("Compilation took {0}ms.", std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

		std::vector<std::filesystem::path> objectPaths = m_Options.Files;
		for (auto& path : objectPaths)
			path = m_Options.OutputDir / CompilerOptions::IntermediateDirectory / path.filename().replace_extension(".o");

		Linker::Link(m_Options.OutputDir, objectPaths);

		DY_LOG_TRACE("Linking took {0}ms.", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - endTime).count());
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
		if (line != 1)
			start++;

		// Find the end of the line
		size_t end = m_FileContent.find_first_of("\n\r", start);
		if (end == std::string::npos)
			end = m_FileContent.size();

		std::string lineStr = m_FileContent.substr(start, end - start);
		std::string& result = Utils::RemoveFrontIndentation(lineStr);
		return result;
	}

}
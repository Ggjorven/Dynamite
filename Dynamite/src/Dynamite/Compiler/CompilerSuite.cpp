#include "dypch.h"
#include "CompilerSuite.hpp"

#include "Dynamite/Core/Logging.hpp"

namespace Dynamite
{

	CompilerSuite::CompilerSuite(const CompilerOptions& options)
		: m_Options(options)
	{
		// Note: IncludeDirs & Multiple files are not supported

		std::vector<std::string> files = options.Get(CompilerFlag::Type::File);
		if (files.size() == 0)
		{
			DY_LOG_ERROR("Compilation terminated, no files specified.");
			return;
		}

		std::ifstream file(files[0]);
		std::stringstream contentBuffer;
		contentBuffer << file.rdbuf();
		std::string content = contentBuffer.str();

		m_Tokenizer = Pulse::Unique<Tokenizer>::Create(content);
		std::vector<Token> tokens = m_Tokenizer->GetTokens();
		
		m_Parser = Pulse::Unique<Parser>::Create(tokens);
		Node::Program program = m_Parser->GetProgram();
	}

	CompilerSuite::~CompilerSuite()
	{
	}

	std::string CompilerSuite::Compile()
	{
		return std::string();
	}

}
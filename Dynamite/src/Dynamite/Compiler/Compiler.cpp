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
	}

	Compiler& Compiler::Get()
	{
		return *s_Instance;
	}

}
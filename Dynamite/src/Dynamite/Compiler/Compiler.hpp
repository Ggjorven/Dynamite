#pragma once

#include "Dynamite/Core/Core.hpp"
#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerOptions.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#include <cstdint>
#include <string>

namespace Dynamite
{

	class Compiler
	{
	public:
		enum class State : uint8_t { Tokenizing, Parsing, Generating };
	public:
		// Constructors
		Compiler(const CompilerOptions& options);
		~Compiler();

		// Methods
		void Compile();

		// Getters & Setters
		inline CompilerOptions& GetOptions() { return m_Options; }
		inline const CompilerOptions& GetOptions() const { return m_Options; }

		static Compiler& Get();

	private:
		CompilerOptions m_Options;
	};

}
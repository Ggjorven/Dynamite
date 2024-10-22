#pragma once

#include "Dynamite/Core/Core.hpp"
#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerOptions.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#include <cstdint>
#include <string>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Compiler
	/////////////////////////////////////////////////////////////////
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
		static Compiler& Get();

		inline static CompilerOptions& GetOptions() { return Get().m_Options; }

	public:
		// Logging
		template<typename ...Args> static void Print(LogLevel logLevel, const std::string& fmt, Args&& ...args);
		template<typename ...Args> static void Print(LogLevel logLevel, uint32_t line, const std::string& fmt, Args&& ...args);

		template<typename ...Args> inline static void Warn(const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Warn, fmt, std::forward<Args>(args)...); }
		template<typename ...Args> inline static void Warn(uint32_t line, const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Warn, line, fmt, std::forward<Args>(args)...); }

		template<typename ...Args> inline static void Error(const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Error, fmt, std::forward<Args>(args)...); }
		template<typename ...Args> inline static void Error(uint32_t line, const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Error, line, fmt, std::forward<Args>(args)...); }

	private:
		// Private methods
		const std::string GetCurrentLine(uint32_t line) const;

	private:
		State m_State;
		CompilerOptions m_Options;

		std::filesystem::path m_File = {};
		std::string m_FileContent = {};
		std::vector<Token> m_Tokens = { };
	};

	/////////////////////////////////////////////////////////////////
	// Templated functions
	/////////////////////////////////////////////////////////////////
	template<typename ...Args>
	void Compiler::Print(LogLevel logLevel, const std::string& fmt, Args && ...args)
	{
		std::string str = Pulse::Text::Format(fmt, std::forward<Args>(args)...);
		Logger::LogMessage(logLevel, "While {0}:\n    {1}", Pulse::Enum::Name(Get().m_State), str);
	}

	template<typename ...Args>
	void Compiler::Print(LogLevel logLevel, uint32_t line, const std::string& fmt, Args&& ...args)
	{
		std::string str = Pulse::Text::Format(fmt, std::forward<Args>(args)...);
		Logger::LogMessage(logLevel, "While {0}:\n    {1}\n\n    Line: {2}\n    File: {3}:{4}", Pulse::Enum::Name(Get().m_State), str, Get().GetCurrentLine(line), Get().m_File.string(), line);
	}

}
#pragma once

#include "Dynamite/Core/Core.hpp"
#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerOptions.hpp"

#include "Dynamite/Dynamite/Tokenizer/Token.hpp"
#include "Dynamite/Language/Nodes/Program.hpp"
#include "Dynamite/Generator/Generator.hpp"

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
		enum class State : uint8_t { Preprocessing, Tokenizing, Parsing, Generating, Linking };
	public:
		// Constructors
		Compiler(const CompilerOptions& options);
		~Compiler();

		// Methods
		void Compile();

		// Getters & Setters
		static Compiler& Get();
		inline static size_t GetErrorCount() { return Get().m_ErrorCount; }
		inline static CompilerOptions& GetOptions() { return Get().m_Options; }

	public:
		// Logging
		template<typename ...Args> static void Print(LogLevel logLevel, const std::string& fmt, Args&& ...args);
		template<typename ...Args> static void Print(LogLevel logLevel, uint32_t line, const std::string& fmt, Args&& ...args);

		template<typename ...Args> inline static void Warn(const std::string& fmt, Args&& ...args);
		template<typename ...Args> inline static void Warn(uint32_t line, const std::string& fmt, Args&& ...args);

		template<typename ...Args> inline static void Error(const std::string& fmt, Args&& ...args);
		template<typename ...Args> inline static void Error(uint32_t line, const std::string& fmt, Args&& ...args);

	private:
		// Private methods
		std::string GetCurrentLine(uint32_t line) const;

	private:
		State m_State;
		CompilerOptions m_Options;

		size_t m_ErrorCount = 0;

		std::filesystem::path m_CurrentFile = {};

		std::string m_FileContent = {};
		std::vector<Token> m_Tokens = { };
		Language::Node::Program m_Program = {};
		std::string m_GeneratedIR = {};
	};

	/////////////////////////////////////////////////////////////////
	// Templated functions
	/////////////////////////////////////////////////////////////////
	template<typename ...Args>
	void Compiler::Print(LogLevel logLevel, const std::string& fmt, Args && ...args)
	{
		std::string str = Pulse::Text::Format(fmt, std::forward<Args>(args)...);
		Logger::LogMessage(logLevel, "While {0}:\n    {1}\n", Pulse::Enum::Name(Get().m_State), str);
	}
	template<typename ...Args>
	void Compiler::Print(LogLevel logLevel, uint32_t line, const std::string& fmt, Args&& ...args)
	{
		std::string str = Pulse::Text::Format(fmt, std::forward<Args>(args)...);
		Logger::LogMessage(logLevel, "While {0}:\n    {1}\n\n    Line: {2}\n    File: {3}:{4}\n", Pulse::Enum::Name(Get().m_State), str, Get().GetCurrentLine(line), Get().m_CurrentFile.string(), line);
	}

	template<typename ...Args>
	void Compiler::Warn(const std::string& fmt, Args&& ...args)
	{ 
		Print<Args...>(LogLevel::Warn, fmt, std::forward<Args>(args)...); 
	}
	template<typename ...Args> 
	void Compiler::Warn(uint32_t line, const std::string& fmt, Args&& ...args)
	{ 
		Print<Args...>(LogLevel::Warn, line, fmt, std::forward<Args>(args)...); 
	}

	template<typename ...Args>
	void Compiler::Error(const std::string& fmt, Args&& ...args)
	{
		Print<Args...>(LogLevel::Error, fmt, std::forward<Args>(args)...);
		
		Get().m_ErrorCount++;
	}
	template<typename ...Args>
	void Compiler::Error(uint32_t line, const std::string& fmt, Args&& ...args)
	{
		Print<Args...>(LogLevel::Error, line, fmt, std::forward<Args>(args)...);

		Get().m_ErrorCount++;
	}

}
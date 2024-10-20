#pragma once

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"
#include "Dynamite/Parsing/Parser.hpp"
#include "Dynamite/Generator/Generator.hpp"

#include "Dynamite/Compiler/CompilerOptions.hpp"

#include <Pulse/Core/Unique.hpp>
#include <Pulse/Text/Format.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#include <cstdint>
#include <string>
#include <optional>

namespace Dynamite
{

	class CompilerSuite
	{
	public:
		enum class State : uint8_t { Tokenizing, Parsing, Generating };
	public:
		CompilerSuite(const CompilerOptions& options);
		~CompilerSuite();

		void Compile();

		inline const Tokenizer* GetTokenizer() const { return m_Tokenizer.Raw(); }
		inline const Parser* GetParser() const { return m_Parser.Raw(); }

		inline CompilerOptions& GetOptions() { return m_Options; }
		inline const CompilerOptions& GetOptions() const { return m_Options; }

		static CompilerSuite& Get();

	public:
		const std::string GetCurrentLine(uint32_t line) const;

		inline const State GetState() const { return m_CurrentState; }
		inline const std::filesystem::path& GetCurrentFile() const { return m_CurrentFile; }

	public:
		template<typename ...Args>
		static void Print(LogLevel logLevel, const std::string& fmt, Args&& ...args);
		template<typename ...Args>
		static void Print(LogLevel logLevel, uint32_t line, const std::string& fmt, Args&& ...args);

		template<typename ...Args>
		static void Warn(const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Warn, fmt, std::forward<Args>(args)...); }
		template<typename ...Args>
		static void Warn(uint32_t line, const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Warn, line, fmt, std::forward<Args>(args)...); }
		
		template<typename ...Args>
		static void Error(const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Error, fmt, std::forward<Args>(args)...); }
		template<typename ...Args>
		static void Error(uint32_t line, const std::string& fmt, Args&& ...args) { Print<Args...>(LogLevel::Error, line, fmt, std::forward<Args>(args)...); }

	private:
		CompilerOptions m_Options;
		State m_CurrentState = State::Tokenizing;

		// State machines
		Pulse::Unique<Tokenizer> m_Tokenizer = nullptr;
		Pulse::Unique<Parser> m_Parser = nullptr;
		Pulse::Unique<Generator> m_Generator = nullptr;

		// File specific content
		std::filesystem::path m_CurrentFile = {};
		std::string m_CurrentFileContent = {};
		std::vector<Token> m_CurrentTokens = { };
		Node::Program m_CurrentProgram = {};
	};

	template<typename ...Args>
	inline void CompilerSuite::Print(LogLevel logLevel, const std::string& fmt, Args && ...args)
	{
		CompilerSuite& instance = Get();

		std::string str = Pulse::Text::Format(fmt, std::forward<Args>(args)...);
		Logger::LogMessage(logLevel, "While {0}:\n    {1}", Pulse::Enum::Name(instance.GetState()), str);
	}

	template<typename ...Args>
	void CompilerSuite::Print(LogLevel logLevel, uint32_t line, const std::string& fmt, Args&& ...args)
	{
		CompilerSuite& instance = Get();

		std::string str = Pulse::Text::Format(fmt, std::forward<Args>(args)...);
		Logger::LogMessage(logLevel, "While {0}:\n    {1}\n\n    Line: {2}\n    File: {3}:{4}", Pulse::Enum::Name(instance.GetState()), str, instance.GetCurrentLine(line), instance.GetCurrentFile().string(), line);
	}

}
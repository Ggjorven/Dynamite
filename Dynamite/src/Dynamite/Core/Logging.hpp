#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <string>
#include <string_view>
#include <memory>

#include <Pulse/Core/Core.hpp>
#include <Pulse/Text/Format.hpp>

namespace Dynamite
{

	enum class LogLevel : uint8_t
	{
		None = 0, Trace, Info, Warn, Error, Fatal
	};

	class Logger
	{
	public:
		static void Init();

		template<typename ... Args>
		static void LogMessage(LogLevel level, std::string_view fmt, const Args& ...args);

		#if !defined(DY_CONFIG_DIST)
			#define DY_LOG_TRACE(...)	::Dynamite::Logger::LogMessage(::Dynamite::LogLevel::Trace, __VA_ARGS__)
			#define DY_LOG_INFO(...)	::Dynamite::Logger::LogMessage(::Dynamite::LogLevel::Info, __VA_ARGS__)
			#define DY_LOG_WARN(...)	::Dynamite::Logger::LogMessage(::Dynamite::LogLevel::Warn, __VA_ARGS__)
			#define DY_LOG_ERROR(...)	::Dynamite::Logger::LogMessage(::Dynamite::LogLevel::Error, __VA_ARGS__)
			#define DY_LOG_FATAL(...)	::Dynamite::Logger::LogMessage(::Dynamite::LogLevel::Fatal, __VA_ARGS__)
		#else
			#define DY_LOG_TRACE(...)
			#define DY_LOG_INFO(...)
			#define DY_LOG_WARN(...)
			#define DY_LOG_ERROR(...)
			#define DY_LOG_FATAL(...)
		#endif

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
		static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> s_ConsoleSink;
	};

	template<typename ... Args>
	void Logger::LogMessage(LogLevel level, std::string_view fmt, const Args& ...args)
	{
		switch (level)
		{
		case LogLevel::Trace:
			spdlog::trace(Pulse::Text::Format(fmt, args...));
			break;
		case LogLevel::Info:
			spdlog::info(Pulse::Text::Format(fmt, args...));
			break;
		case LogLevel::Warn:
			spdlog::warn(Pulse::Text::Format(fmt, args...));
			break;
		case LogLevel::Error:
			spdlog::error(Pulse::Text::Format(fmt, args...));
			break;
		case LogLevel::Fatal:
			spdlog::critical(Pulse::Text::Format(fmt, args...));
			break;

		default:
			break;
		}
	}

	#ifndef DY_CONFIG_DIST
		#define DY_VERIFY(value, ...) \
			if (!(value)) \
			{ \
				DY_LOG_FATAL(__VA_ARGS__); \
			}
	#else
		#define DY_VERIFY(value, ...)
	#endif

	#ifdef DY_CONFIG_DEBUG
		// Note: Don't put function calls in DY_ASSERT! They don't run in DIST
		#define DY_ASSERT(value, ...) \
			if (!(value)) \
			{ \
				DY_LOG_FATAL(__VA_ARGS__); \
				PULSE_DEBUG_BREAK(); \
			}
	#elif defined(HZ_CONFIG_RELEASE)
		// Note: Don't put function calls in DY_ASSERT! They don't run in DIST
		#define DY_ASSERT(value, ...) \
			if (!(value)) \
			{ \
				DY_LOG_FATAL(__VA_ARGS__); \
			}
	#else
		// Note: Don't put function calls in DY_ASSERT! They don't run in DIST
		#define DY_ASSERT(value, ...)
	#endif

}

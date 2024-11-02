#include "dypch.h"
#include "Logging.hpp"

#include <Pulse/Core/Logging.hpp>

namespace Dynamite
{

	std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Logger::s_ConsoleSink = nullptr;
	std::shared_ptr<spdlog::logger> Logger::s_Logger = nullptr;

	namespace
	{
		static void PulseLogCallback(Pulse::LogLevel level, std::string message)
		{
			if (level == Pulse::LogLevel::None) return;

			Logger::LogMessage((Dynamite::LogLevel)level, message);
		}

		static void PulseAssertCallback(bool success, std::string message)
		{
			DY_ASSERT(success, message);
		}
	}



	void Logger::Init()
	{
		s_ConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_ConsoleSink->set_pattern("%v%$");

		s_Logger = std::make_shared<spdlog::logger>("Dynamite Logger", s_ConsoleSink);
		spdlog::set_default_logger(s_Logger);
		spdlog::set_level(spdlog::level::trace);

		// Initialize Pulse logger
		Pulse::Logger::Init(&PulseLogCallback, &PulseAssertCallback);
	}

}

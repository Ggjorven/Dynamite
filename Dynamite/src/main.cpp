#include "dypch.h"

#include "Dynamite/Core/Logging.hpp"

using namespace Dynamite;

int main(int argc, char* argv[])
{
	Logger::Init();

	DY_LOG_TRACE("Trace message");
	DY_LOG_INFO("Info message");
	DY_LOG_WARN("Warn message");
	DY_LOG_ERROR("Error message");
	DY_LOG_FATAL("Fatal message");

	return 0;
}
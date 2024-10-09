#include "dypch.h"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/CompilerSuite.hpp"

#include <fstream>
#include <sstream>

using namespace Dynamite;

int main(int argc, char* argv[])
{
	Logger::Init();

	CompilerSuite compilerSuite(CompilerOptions(argc, argv));
	std::string result = compilerSuite.Compile();

	return 0;
}
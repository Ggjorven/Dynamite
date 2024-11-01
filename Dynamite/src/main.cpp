#include "dypch.h"

#include "Core/Logging.hpp"

#include "Compiler/Compiler.hpp"
#include "Compiler/CompilerOptions.hpp"

using namespace Dynamite;

int main(int argc, char* argv[])
{
	Logger::Init();

	Compiler compiler(CompilerOptions(argc, argv));
	compiler.Compile();

	return 0;
}
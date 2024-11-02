#include "dypch.h"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"
#include "Dynamite/Compiler/CompilerOptions.hpp"

using namespace Dynamite;

int main(int argc, char* argv[])
{
	Logger::Init();

	Compiler compiler(CompilerOptions(argc, argv));
	compiler.Compile();

	return 0;
}
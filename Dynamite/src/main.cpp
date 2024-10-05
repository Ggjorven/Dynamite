#include "dypch.h"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"
#include "Dynamite/Parsing/Parser.hpp"
#include "Dynamite/Generator/Generator.hpp"

#include <fstream>
#include <sstream>

using namespace Dynamite;

int main(int argc, char* argv[])
{
	Logger::Init();

	// Check for right amount of args
	if (argc == 1)
	{
		DY_LOG_ERROR("Invalid usage.");
		DY_LOG_ERROR("Example usage: ./Dynamite <input.dy>");
		return EXIT_FAILURE;
	}

	std::string contents;
	{
		std::stringstream contentsStream;
		std::fstream input(argv[1], std::ios::in);

		contentsStream << input.rdbuf();
		contents = contentsStream.str();
	}

	Tokenizer tokenizer(std::move(contents));
	std::vector<Token> tokens = tokenizer.GetTokens();
	
	DY_LOG_TRACE("-------------------------------------");
	DY_LOG_TRACE("-- Tokens found");
	DY_LOG_TRACE("-------------------------------------");
	Tokenizer::Print(tokens);

	Parser parser(tokens);
	Node::Program program = parser.GetProgram();

	DY_LOG_TRACE("-------------------------------------");
	DY_LOG_TRACE("-- Nodes created");
	DY_LOG_TRACE("-------------------------------------");
	Parser::Print(program);

	return EXIT_SUCCESS;
}
#include "dypch.h"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"
#include "Dynamite/Parsing/Parser.hpp"
#include "Dynamite/Generator/Generator.hpp"

#include <fstream>
#include <sstream>

using namespace Dynamite;

// Temporary function
std::string TokensToAsm(const std::vector<Token>& tokens)
{
	std::stringstream output;

	output << "global _start\n_start:\n";

	for (size_t i = 0; i < tokens.size(); i++) 
	{
		const Token& token = tokens.at(i);

		if (token.Type == TokenType::Exit) 
		{
			if (i + 1 < tokens.size() && tokens.at(i + 1).Type == TokenType::Int64Literal) 
			{
				if (i + 2 < tokens.size() && tokens.at(i + 2).Type == TokenType::Semicolon) 
				{
					output << "    mov rax, 60\n";
					output << "    mov rdi, " << tokens.at(i + 1).Value.value() << "\n";
					output << "    syscall";
				}
			}
		}
	}

	return output.str();
}

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

	Parser parser(std::move(tokens));
	Nodes::Program program = parser.GetProgram();
	
	DY_LOG_TRACE("");
	DY_LOG_TRACE("-------------------------------------");
	DY_LOG_TRACE("-- Nodes created");
	DY_LOG_TRACE("-------------------------------------");
	Parser::Print(program);

	Generator generator(program);
	std::string assembly = generator.Generate();

	DY_LOG_TRACE("");
	DY_LOG_TRACE("-------------------------------------");
	DY_LOG_TRACE("-- Assembly generated");
	DY_LOG_TRACE("-------------------------------------");
	DY_LOG_TRACE("File generated: \n{0}", assembly);

	return EXIT_SUCCESS;
}
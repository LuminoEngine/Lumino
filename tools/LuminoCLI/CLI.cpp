
#include "CLI.hpp"

void CLI::output(const ln::String & str)
{
	std::cout << str << std::endl;
}

void CLI::message(const ln::String & str)
{
	std::cerr << str << std::endl;
}

void CLI::warning(const ln::String & str)
{
	std::cerr << str << std::endl;
}

void CLI::error(const ln::String & str)
{
	std::cerr << str << std::endl;
}

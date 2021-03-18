
#include "CLI.hpp"

void CLI::output(const ln::String & str)
{
	std::cout << str << std::endl;
}

void CLI::verbose(const ln::String& str)
{
	std::cerr << str << std::endl;
}

void CLI::info(const ln::String & str)
{
	std::cerr << str << std::endl;
}

void CLI::warning(const ln::String & str)
{
	std::cerr << "Warning: " << str << std::endl;
}

void CLI::error(const ln::String & str)
{
	std::cerr << "Error: " << str << std::endl;
}

void CLI::fatal(const ln::String& str)
{
    std::cerr << "Fatal error: " << str << std::endl;
    throw ln::Exception(str.c_str());
}

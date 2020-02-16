#pragma once

class FxcCommand
{
public:
	ln::Path outputFile;

    int execute(const ln::Path& inputFile);
};

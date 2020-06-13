#pragma once

class FxcCommand
{
public:
	ln::Path outputFile;
	ln::Path exportDir;

    int execute(const ln::Path& inputFile);
};

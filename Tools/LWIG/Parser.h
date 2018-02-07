
#pragma once

class SymbolDatabase;

class HeaderParser
{
public:
	static int parse(const Path& filePath, ::SymbolDatabase* db);
};


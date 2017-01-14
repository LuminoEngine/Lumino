
#pragma once
#include "../OutputBuffer.h"
class SymbolDatabase;

/** WrapperIFGenerator */
class WrapperIFGenerator
{
public:
	void Generate(SymbolDatabase* database);

private:
	SymbolDatabase*	m_database;
};


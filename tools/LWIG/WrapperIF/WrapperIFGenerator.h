
#pragma once
#include "../OutputBuffer.h"
class SymbolDatabase;

/** WrapperIFGenerator */
class WrapperIFGenerator
{
public:
	void Generate(SymbolDatabase* database);

private:
	StringA MakeInstanceParamName(TypeInfoPtr info);
	StringA MakeMethods(TypeInfoPtr typeInfo);
	StringA MakeParamTypeName(TypeInfoPtr typeInfo, bool isOut);

	SymbolDatabase*	m_database;
};


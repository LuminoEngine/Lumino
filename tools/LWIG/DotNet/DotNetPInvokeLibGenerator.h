#pragma once
#include "../OutputBuffer.h"
class SymbolDatabase;

class DotNetPInvokeLibGenerator
{
public:
	void Generate();

private:
	String MakeParamTypeName(ParameterInfoPtr paramInfo);
};

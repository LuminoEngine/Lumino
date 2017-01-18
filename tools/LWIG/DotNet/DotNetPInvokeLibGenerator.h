#pragma once
#include "../OutputBuffer.h"

class DotNetPInvokeLibGenerator
{
public:
	void Generate();

private:
	String MakeParamTypeName(ParameterInfoPtr paramInfo);
};

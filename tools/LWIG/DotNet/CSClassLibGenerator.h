#pragma once
#include "../OutputBuffer.h"

class CSClassLibGenerator
{
public:
	void Generate();

private:
	String MakeMethodBody(MethodInfoPtr methodInfo, bool isProperty);
	void MakeTypeInfoRegisters(TypeInfoPtr classInfo, OutputBuffer* typeInfoRegistersText, OutputBuffer* typeInfoPInvolesText);
};

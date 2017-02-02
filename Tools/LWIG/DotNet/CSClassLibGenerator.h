#pragma once
#include "../OutputBuffer.h"

class CSClassLibGenerator
{
public:
	void Generate();

private:
	String MakeMethodBody(MethodInfoPtr methodInfo, bool isProperty);
	String MakeOverrideCallbackMethodBody(MethodInfoPtr methodInfo);
	void MakeTypeInfoRegisters(TypeInfoPtr classInfo, OutputBuffer* typeInfoRegistersText, OutputBuffer* typeInfoPInvolesText);

	OutputBuffer	m_fieldsText;
};

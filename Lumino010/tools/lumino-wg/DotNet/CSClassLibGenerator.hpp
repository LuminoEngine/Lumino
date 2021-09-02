#pragma once
#include "../OutputBuffer.h"

class CSClassLibGenerator
{
public:
	void generate();

private:
	String MakeMethodBody(MethodInfoPtr methodInfo, bool isProperty);
	String MakeOverrideCallbackMethodBody(MethodInfoPtr methodInfo);
	void MakeTypeInfoRegisters(TypeInfoPtr classInfo, OutputBuffer* typeInfoRegistersText, OutputBuffer* typeInfoPInvolesText);
	static String MakeMethodHeaderParamList(MethodInfoPtr methodInfo);
	static String MakeEventName(MethodInfoPtr eventConnectMethod);
	static String MakeEventCallbackMethodName(MethodInfoPtr method);

	// file
	OutputBuffer	m_delegatesText;

	// class
	OutputBuffer	m_fieldsText;
	OutputBuffer	m_eventsText;
	OutputBuffer	m_posiInitializeText;
};

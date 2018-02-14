
#pragma once
#include "../Generator.h"

/** */
class WrapperIFClassesGenerator
	: public Generator
{
public:
	void generate();

private:
	String GenerateWrapperIFClasses();
	String GenerateOverrideCallerFuncPtrs();
	String MakeEventWrapperMemberVariableName(Ref<MethodInfo> connectMethod);

	OutputBuffer	m_eventWrapperConnectExps;
	OutputBuffer	m_eventWrappers;
};


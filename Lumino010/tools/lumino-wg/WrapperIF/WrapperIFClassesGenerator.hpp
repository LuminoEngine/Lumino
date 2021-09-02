
#pragma once
#include "../Generator.hpp"

/** */
class WrapperIFClassesGenerator
	: public Generator
{
public:
	void generate();

private:
	ln::String GenerateWrapperIFClasses();
	ln::String GenerateOverrideCallerFuncPtrs();
	ln::String MakeEventWrapperMemberVariableName(ln::Ref<MethodSymbol> connectMethod);

	OutputBuffer	m_eventWrapperConnectExps;
	OutputBuffer	m_eventWrappers;
};


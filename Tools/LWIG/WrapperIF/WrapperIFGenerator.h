
#pragma once
#include "../Generator.h"
class SymbolDatabase;

/** CåæåÍÇÃópÇÃ API ÇèoóÕÇ∑ÇÈ */
class WrapperIFGenerator
	: public Generator
{
public:
	WrapperIFGenerator();
	void generate();

private:
	String GenerateOverrideCallerFuncPtrs();
	String GenerateWrapperIFClasses();

	String MakeInstanceParamName(Ref<TypeInfo> info);
	//String MakeMethods(Ref<TypeInfo> typeInfo);
	String MakeMethodHeader(Ref<MethodInfo> methodInfo);
	String MakeMethod(Ref<TypeInfo> typeInfo, Ref<MethodInfo> methodInfo, bool virtualBase);
	String MakeCApiParamTypeName(Ref<MethodInfo> methodInfo, Ref<ParameterInfo> paramInfo);
	String MakeCppTypeName(Ref<TypeInfo> typeInfo);
	String MakeDocumentComment(Ref<DocumentInfo> doc);

	String MakeEventWrapperMemberVariableName(Ref<MethodInfo> connectMethod);

	OutputBuffer	m_eventWrapperConnectExps;
	OutputBuffer	m_eventWrappers;
};


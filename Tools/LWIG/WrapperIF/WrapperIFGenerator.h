
#pragma once
#include "../OutputBuffer.h"
class SymbolDatabase;

/** CåæåÍÇÃópÇÃ API ÇèoóÕÇ∑ÇÈ */
class WrapperIFGenerator
{
public:
	WrapperIFGenerator(SymbolDatabase* database, const Path& templateDir, const Path& outputDir);
	void generate();

private:
	String GenerateOverrideCallerFuncPtrs();
	String GenerateWrapperIFClasses();

	String MakeInstanceParamName(Ref<TypeInfo> info);
	String MakeMethods(Ref<TypeInfo> typeInfo);
	String MakeMethod(Ref<TypeInfo> typeInfo, Ref<MethodInfo> methodInfo, bool virtualBase);
	String MakeCApiParamTypeName(Ref<MethodInfo> methodInfo, Ref<ParameterInfo> paramInfo);
	String MakeCppTypeName(Ref<TypeInfo> typeInfo);
	String MakeDocumentComment(Ref<DocumentInfo> doc);

	String MakeEventWrapperMemberVariableName(Ref<MethodInfo> connectMethod);

	SymbolDatabase*	m_database;
	Path			m_templateDir;
	Path			m_outputDir;

	OutputBuffer	m_eventWrapperConnectExps;
	OutputBuffer	m_eventWrappers;
};


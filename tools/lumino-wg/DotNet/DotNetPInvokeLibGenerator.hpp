#pragma once
#include "../OutputBuffer.h"

class DotNetPInvokeLibGenerator
{
public:
	void generate();

	String MakePInvokeMethodDecl(MethodInfoPtr methodInfo, bool virtualBase);
	String MakePInvokeMethodDeclSetOverrideCallback(MethodInfoPtr methodInfo);
	static String MakePInvokeMethodDeclParamList(MethodInfoPtr methodInfo);
	static String MakeParamTypeName(ParameterInfoPtr paramInfo);
};

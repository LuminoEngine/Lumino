#pragma once
#include "../Generator.h"

class CSStructsGenerator
	: public Generator
{
public:
	void generate();

private:
	String MakeMethodBody(Ref<MethodInfo> methodInfo, bool isSetProperty);
	bool CheckAllMemberSetConstructor(Ref<TypeInfo> structInfo, Ref<MethodInfo> methodInfo);
};

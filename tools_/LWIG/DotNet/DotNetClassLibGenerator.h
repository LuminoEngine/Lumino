#pragma once
#include "../OutputBuffer.h"

class CSStructsGenerator
{
public:
	void Generate();

private:
	String MakeMethodBody(MethodInfoPtr methodInfo, bool isSetProperty);
	bool CheckAllMemberSetConstructor(TypeInfoPtr structInfo, MethodInfoPtr methodInfo);
};

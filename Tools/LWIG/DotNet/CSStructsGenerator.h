#pragma once
#include "../Generator.h"

class CSStructsGenerator
	: public Generator
{
public:
	void generate();

private:
	String MakeMethodBody(Ref<MethodSymbol> methodInfo, bool isSetProperty);
	bool CheckAllMemberSetConstructor(Ref<TypeSymbol> structInfo, Ref<MethodSymbol> methodInfo);
};

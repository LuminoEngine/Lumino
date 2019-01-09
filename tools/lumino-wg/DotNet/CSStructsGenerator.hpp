#pragma once
#include "../Generator.hpp"

class CSStructsGenerator
	: public Generator
{
public:
	void generate();

private:
	ln::String MakeMethodBody(ln::Ref<MethodSymbol> methodInfo, bool isSetProperty);
	bool CheckAllMemberSetConstructor(ln::Ref<TypeSymbol> structInfo, ln::Ref<MethodSymbol> methodInfo);
};

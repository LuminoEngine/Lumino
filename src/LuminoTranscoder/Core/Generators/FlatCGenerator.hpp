
#pragma once
#include "Generator.hpp"
class SymbolDatabase;

// .h
class FlatCHeaderGenerator
	: public Generator
{
public:
	void generate();

private:
	//ln::String GenerateOverrideCallerFuncPtrs();

	//ln::String MakeInstanceParamName(ln::Ref<TypeSymbol> info);
	////ln::String MakeMethods(Ref<TypeSymbol> typeInfo);
	//ln::String makeFuncHeader(ln::Ref<MethodSymbol> methodInfo);
	//ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);

	//ln::String MakeDocumentComment(ln::Ref<DocumentSymbol> doc);


};


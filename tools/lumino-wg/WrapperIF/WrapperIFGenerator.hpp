
#pragma once
#include "../Generator.hpp"
class SymbolDatabase;

/** C言語の用の API を出力する */
class WrapperIFGenerator
	: public Generator
{
public:
	WrapperIFGenerator();
	void generate();


private:
	ln::String GenerateOverrideCallerFuncPtrs();

	ln::String MakeInstanceParamName(ln::Ref<TypeSymbol> info);
	//ln::String MakeMethods(Ref<TypeSymbol> typeInfo);
	ln::String makeFuncHeader(ln::Ref<MethodSymbol> methodInfo);
	ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);

	ln::String MakeDocumentComment(ln::Ref<DocumentSymbol> doc);


};


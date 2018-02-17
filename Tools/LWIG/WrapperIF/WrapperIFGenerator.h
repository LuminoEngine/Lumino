
#pragma once
#include "../Generator.h"
class SymbolDatabase;

/** CŒ¾Œê‚Ì—p‚Ì API ‚ğo—Í‚·‚é */
class WrapperIFGenerator
	: public Generator
{
public:
	WrapperIFGenerator();
	void generate();


private:
	String GenerateOverrideCallerFuncPtrs();

	String MakeInstanceParamName(Ref<TypeSymbol> info);
	//String MakeMethods(Ref<TypeSymbol> typeInfo);
	String makeFuncHeader(Ref<MethodSymbol> methodInfo);
	String makeFuncBody(Ref<TypeSymbol> typeInfo, Ref<MethodSymbol> methodInfo);

	String MakeDocumentComment(Ref<DocumentSymbol> doc);


};


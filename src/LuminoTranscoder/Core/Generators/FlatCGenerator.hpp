
#pragma once
#include "Generator.hpp"
class SymbolDatabase;

class FlatCCommon
{
public:
	static ln::String makeFlatCDeclTypeName(GeneratorConfiguration* config, TypeSymbol* typeInfo);


	static ln::String makeCppQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCParamQualTypeName(ln::Ref<MethodSymbol> methodInfo, ln::Ref<ParameterSymbol> paramInfo);
};

// .h
class FlatCHeaderGenerator
	: public Generator
{
public:
	void generate();

private:
	ln::String makeDocumentComment(DocumentInfo* doc);

	//ln::String GenerateOverrideCallerFuncPtrs();

	//ln::String MakeInstanceParamName(ln::Ref<TypeSymbol> info);
	////ln::String MakeMethods(Ref<TypeSymbol> typeInfo);
	//ln::String makeFuncHeader(ln::Ref<MethodSymbol> methodInfo);
	//ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);



};


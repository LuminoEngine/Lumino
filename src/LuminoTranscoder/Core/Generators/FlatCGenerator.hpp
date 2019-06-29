
#pragma once
#include "Generator.hpp"
class SymbolDatabase;

class FlatCCommon
{
public:
	static ln::String makeFuncName(GeneratorConfiguration* config, MethodSymbol* method);
	static ln::String makeFlatCDeclTypeName(GeneratorConfiguration* config, TypeSymbol* typeInfo);
	static ln::String makeFunkParamList(MethodSymbol* method);
	static ln::String makeFuncHeader(GeneratorConfiguration* config, MethodSymbol* methodInfo);

	static ln::String makeCppQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCParamQualTypeName(GeneratorConfiguration* config, ln::Ref<MethodSymbol> methodInfo, ln::Ref<MethodParameterSymbol> paramInfo);
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
	//ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);



};


// .cpp
class FlatCSourceGenerator
	: public Generator
{
public:
	void generate();

private:
	ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);
};



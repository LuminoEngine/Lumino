
#pragma once
#include "Generator.hpp"
class SymbolDatabase;

enum class FlatCharset
{
	Unicode,
	Ascii,
	//Wide,
};

class FlatCCommon
{
public:
	static ln::String makeCharTypeName(FlatCharset charset);

	// e.g.) LnEngine_Init
	static ln::String makeFuncName(GeneratorConfiguration* config, MethodSymbol* method, FlatCharset charset);

	// 修飾無しの型名
	static ln::String makeTypeName(GeneratorConfiguration* config, TypeSymbol* type);

	static ln::String makeFunkParamList(MethodSymbol* method);
	static ln::String makeFuncHeader(GeneratorConfiguration* config, MethodSymbol* methodInfo, FlatCharset charset);

	static ln::String makeEnumMemberName(GeneratorConfiguration* config, TypeSymbol* enumType, ConstantSymbol* member);


	static ln::String makeCppQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCParamQualTypeName(GeneratorConfiguration* config, ln::Ref<MethodSymbol> methodInfo, ln::Ref<MethodParameterSymbol> paramInfo, FlatCharset charset);
	static ln::String makeInstanceParamName(TypeSymbol* type);	// this にあたる引数名
};

// .h
class FlatCHeaderGenerator
	: public Generator
{
public:
	void generate();

private:
	ln::String makeDocumentComment(DocumentInfo* doc) const;

	//ln::String GenerateOverrideCallerFuncPtrs();

	//ln::String MakeInstanceParamName(ln::Ref<TypeSymbol> info);
	////ln::String MakeMethods(Ref<TypeSymbol> typeInfo);
	//ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);

	ln::String makeEnumDecls() const;

};


// .cpp
class FlatCSourceGenerator
	: public Generator
{
public:
	void generate();

private:
	ln::String makeFlatFullName(TypeSymbol* type) const { return type->fullName().replace(u"::", u"_"); }	// ln::Texture -> ln_Texture

	ln::String makeWrapSubclassDecls() const;
	ln::String makeWrapSubclassName(TypeSymbol* type) const { return u"LNWS_" + makeFlatFullName(type); }

	ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo);
	ln::String makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset);
};



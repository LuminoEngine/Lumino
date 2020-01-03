
#pragma once
#include "Generator.hpp"
class SymbolDatabase;

class FlatCCommon
    : public Generator
{
public:

	// e.g.) LnEngine_Init
	//static ln::String makeFuncName(GeneratorConfiguration* config, MethodSymbol* method, FlatCharset charset);

	// 修飾無しの型名
	//static ln::String makeTypeName(GeneratorConfiguration* config, TypeSymbol* type);

	//static ln::String makeFunkParamList(MethodSymbol* method);

	static ln::String makeEnumMemberName(GeneratorConfiguration* config, TypeSymbol* enumType, ConstantSymbol* member);


	static ln::String makeCppQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeFlatCQualTypeName(TypeSymbol* typeInfo);
	static ln::String makeInstanceParamName(TypeSymbol* type);	// this にあたる引数名

    ln::String makeCreateDelegateObjectFuncHeader(TypeSymbol* delegateSymbol) const;
    ln::String makeGetTypeInfoIdFuncName(TypeSymbol* classType) const { return makeFlatClassName(classType) + u"_GetTypeInfoId"; }
    ln::String makeGetTypeInfoIdFuncHeader(TypeSymbol* classType) const { return u"extern LN_API int " + makeGetTypeInfoIdFuncName(classType) + u"()"; }
};

// .h
class FlatCHeaderGenerator
	: public FlatCCommon
{
public:
	void generate();

private:

	ln::String makeDocumentComment(DocumentInfo* doc) const;
	ln::String makeMethodDocumentComment(const MethodSymbol* method) const;
    ln::String makeDelegateFuncPtrDecl(const TypeSymbol* delegateSymbol) const;

	ln::String makeEnumDecls() const;

};


// .cpp
class FlatCSourceGenerator
	: public FlatCCommon
{
public:
	void generate();

private:
	ln::String generateDelegateObjects() const;

	ln::String makeFlatFullName(const TypeSymbol* type) const { return type->fullName().replace(u"::", u"_"); }	// ln::Texture -> ln_Texture

	ln::String makeWrapSubclassDecls() const;
	ln::String makeWrapSubclassName(const TypeSymbol* type) const { return u"LNWS_" + makeFlatFullName(type); }
	ln::String makeNativeParamList(const MethodSymbol* method) const;
	ln::String makeNativeArgList(const MethodSymbol* method) const;
	ln::String makeFlatArgList(const MethodSymbol* method) const;

	ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset);
	//ln::String makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset);
	ln::String makeEventConnectorFuncBody(const TypeSymbol* classInfo, const MethodSymbol* methodInfo) const;
};



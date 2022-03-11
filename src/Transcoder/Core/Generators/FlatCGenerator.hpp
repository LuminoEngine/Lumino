
#pragma once
#include "Generator.hpp"
class SymbolDatabase;

class FlatCCommon
    : public Generator {
public:
    // e.g.) LnEngine_Init
    //static ln::String makeFuncName(GeneratorConfiguration* config, MethodSymbol* method, FlatCharset charset);

    // 修飾無しの型名
    //static ln::String makeTypeName(GeneratorConfiguration* config, TypeSymbol* type);

    //static ln::String makeFunkParamList(MethodSymbol* method);

    static ln::String makeCppQualTypeName(TypeSymbol* typeInfo);
    static ln::String makeFlatCQualTypeName(TypeSymbol* typeInfo);
    static ln::String makeInstanceParamName(TypeSymbol* type); // this にあたる引数名

    ln::String makeCreateDelegateObjectFuncHeader(const TypeSymbol* delegateSymbol) const;
    ln::String makeGetTypeInfoIdFuncName(TypeSymbol* classType) const { return makeFlatClassName(classType) + U"_GetTypeInfoId"; }
    ln::String makeGetTypeInfoIdFuncHeader(TypeSymbol* classType) const { return U"extern LN_FLAT_API int " + makeGetTypeInfoIdFuncName(classType) + U"()"; }
};

// .h
class FlatCHeaderGenerator
    : public FlatCCommon {
public:
    void generate();

private:
    ln::String makeDocumentComment(DocumentInfo* doc) const;
    ln::String makeMethodDocumentComment(const MethodSymbol* method) const;
    ln::String makeDelegateFuncPtrDecl(const TypeSymbol* delegateSymbol) const;
    ln::String makeEnumDecls() const;
    ln::String makeSubClassRegistrationInfo(const TypeSymbol* classSymbol) const;
};

// .cpp
class FlatCSourceGenerator
    : public FlatCCommon {
public:
    void generate();

private:
    //ln::String generateDelegateObjects() const;
    ln::String generateWrapSubclassDecls() const;
    ln::String makeWrapSubclassDecl(const TypeSymbol* classSymbol) const;

    ln::String makeFlatFullName(const TypeSymbol* type) const { return type->fullName().replace(U"::", U"_"); } // ln::Texture -> ln_Texture

    ln::String makeWrapSubclassName(const TypeSymbol* type) const { return U"LNWS_" + makeFlatFullName(type); }
    ln::String makeNativeParamList(const MethodSymbol* method) const;
    ln::String makeNativeArgList(const MethodSymbol* method) const;
    ln::String makeOverridePrototypesStructDecl(const TypeSymbol* classSymbol) const;

    ln::String makeFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset);
    //ln::String makeCharsetWrapperFuncBody(ln::Ref<TypeSymbol> typeInfo, ln::Ref<MethodSymbol> methodInfo, FlatCharset charset);
    //ln::String makeEventConnectorFuncBody(const TypeSymbol* classInfo, const MethodSymbol* methodInfo) const;

    ln::String makeSubinstanceAllocStmt() const {
        return ln::String(U"if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));");
    }
    ln::String makeSubinstanceFreeStmt() const {
        return ln::String(U"if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);");
    }

    ln::String makeBaseCallerExpr(const TypeSymbol* classSymbol, const MethodSymbol* methodSymbol) const;
};

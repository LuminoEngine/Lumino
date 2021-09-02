#pragma once
#include "FlatCGenerator.hpp"
#include "Generator.hpp"

/*
    Ruby のクラスは以下の要素でワンセット
    - フィールドを保持する struct (wrapStruct)
	- rb_define_class_under() で登録したクラス情報を保持する static 変数
	- メモリ確保関数 (alloc)
		- alloc → rubyオブジェクトにラップ(mark、delete 登録) → 0クリア の定型文
	- メモリ解放・デストラクト関数 (delete)
		- RefObj なら Release → free
	- GCマーク関数 (mark)
		- wrapStruct の VALUE 型を mark
*/

class RubyGeneratorBase
	: public Generator
{
protected:
	ln::String makeSnakeStyleName(const ln::String& name) const;
	ln::String makeRubyMethodName(MethodSymbol* method) const;
};

class RubyExtGenerator
    : public RubyGeneratorBase
{
public:
    void generate();

private:

	// Namings
	ln::String makeRubyClassInfoVariableName(const TypeSymbol* type) const { return u"g_class_" + type->shortName(); }
    ln::String makeWrapStructName(const TypeSymbol* type) const;    // e.g.) => Wrap_LnTexture2D, LnVector3
	ln::String makeWrapFuncName(MethodSymbol* method) const { return u"Wrap_" + makeFlatFullFuncName(method, FlatCharset::Unicode); }
	ln::String makeWrapFuncName_CallOverrideBase(MethodSymbol* method) const { return u"Wrap_" + makeFlatFullFuncName(method, FlatCharset::Unicode) + u"_CallOverrideBase"; }
	ln::String makeSignalValueName(const MethodSymbol* eventConnectorMethod) const { return eventConnectorMethod->shortName() + u"_Signal"; }
	ln::String makeEventConnectValueName(const MethodSymbol* eventConnectorMethod) const { return eventConnectorMethod->shortName() + u"_EventConnect"; }
	ln::String makeAccessorCacheName(const MethodSymbol* method) const { return makeFlatFullFuncName(method, FlatCharset::Ascii) + u"_AccessorCache"; }
	ln::String makeWrapFuncName_OverrideCallback(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFlatFullFuncName(classSymbol, method, FlatCharset::Unicode) + u"_OverrideCallback"; }
	ln::String makeWrapFuncName_SetOverrideCallback(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFlatFullFuncName(classSymbol, method, FlatCharset::Unicode) + u"_SetOverrideCallback"; }
	ln::String makeWrapFuncName_ProcCaller(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFlatFullFuncName(classSymbol, method, FlatCharset::Unicode) + u"_ProcCaller"; }
	ln::String makeWrapFuncName_SignalCaller(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFlatFullFuncName(classSymbol, method, FlatCharset::Unicode) + u"_SignalCaller"; }

	ln::String makeClassRequiredImplementation(const TypeSymbol* classSymbol) const;
	ln::String makeClassImplementation(const TypeSymbol* classSymbol) const;

	ln::String makeAccessorCacheFieldDecls(const TypeSymbol* classSymbol) const;
	ln::String makeAccessorCacheFieldMarks(const TypeSymbol* classSymbol) const;
	ln::String makeWrapFuncImplement(const TypeSymbol* classSymbol, const MethodOverloadInfo* overloadInfo) const;
	ln::String makeWrapFuncCallBlock(const TypeSymbol* classSymbol, const MethodSymbol* method) const;
    ln::String makeWrapFuncCallBlock_FieldAccessor(const TypeSymbol* classSymbol, const MethodSymbol* method) const;
	ln::String makeWrapFuncCallBlock_DelegateObjectConstructor(const TypeSymbol* classSymbol, const MethodSymbol* method) const;
	ln::String makeWrapFuncCallBlock_DelegateObjectSetter(const TypeSymbol* classSymbol, const MethodSymbol* method) const;
	ln::String makeVALUEReturnExpr(const TypeSymbol* type, const MethodSymbol* method, const ln::String& varName) const;
	ln::String makeTypeCheckExpr(const TypeSymbol* type, const ln::String& varName) const;
	ln::String makeVALUEToNativeCastExpr(const MethodParameterSymbol* param, const ln::String& varName) const;
	ln::String makeVALUEToNativeCastDecl(const MethodParameterSymbol* param) const;
    ln::String makeNativeToVALUECastDecl(const MethodParameterSymbol* param) const;


	ln::String makeWrapFuncImplement_ProcCaller(const MethodSymbol* delegateProtoType) const;
	ln::String makeWrapFuncImplement_SetOverrideCallback(const TypeSymbol* classInfo) const;
	ln::String makeWrapFuncImplement_SignalCaller(const MethodSymbol* method) const;
	//ln::String makeWrapFuncImplement_EventConnector(const MethodSymbol* method) const;


	ln::String makeEnumTypeVALUEName(TypeSymbol* type) const { return u"g_enum_" + type->shortName(); }
	ln::String makeEnumTypeVALUEVariableDecls() const;
	ln::String makeEnumTypeRegisterCode() const;

	//ln::String makeEventSignalDefinition() const;

	ln::String m_RubyRequiredStructMethodsTemplate;
	ln::String m_RubyRequiredClassMethodsTemplate;
};

class RubyYARDOCSourceGenerator
	: public RubyGeneratorBase
{
public:
	void generate();

private:
	ln::String makeMethodDoc(const MethodOverloadInfo* overloadInfo) const;
	ln::String makeRubyTypeFullName(const TypeSymbol* type) const;
	ln::String translateText(const ln::String& text) const;
	ln::String convertTypeNameToRuby(const TypeSymbol* type) const;
};

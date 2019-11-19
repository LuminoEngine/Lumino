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

class RubyExtGenerator
    : public Generator
{
public:
    void generate();

private:
	ln::String makeSnakeStyleName(const ln::String& name) const;


	ln::String makeRubyClassInfoVariableName(TypeSymbol* type) const { return u"g_class_" + type->shortName(); }
	ln::String makeRubyMethodName(MethodSymbol* method) const;
	ln::String makeWrapStructName(TypeSymbol* type) const { return u"Wrap_" + type->shortName(); }
	ln::String makeWrapFuncName(MethodSymbol* method) const { return u"Wrap_" + makeFuncName(method, FlatCharset::Unicode); }
	ln::String makeWrapFuncName_CallOverrideBase(MethodSymbol* method) const { return u"Wrap_" + makeFuncName(method, FlatCharset::Unicode) + u"_CallOverrideBase"; }
	ln::String makeSignalValueName(const MethodSymbol* eventConnectorMethod) const { return eventConnectorMethod->shortName() + u"_Signal"; }
	ln::String makeEventConnectValueName(const MethodSymbol* eventConnectorMethod) const { return eventConnectorMethod->shortName() + u"_EventConnect"; }
	//ln::String makeEventSignalClassName(const TypeSymbol* delegateType) const;

	ln::String makeWrapFuncName_OverrideCallback(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFuncName(classSymbol, method, FlatCharset::Unicode) + u"_OverrideCallback"; }
	ln::String makeWrapFuncName_SetOverrideCallback(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFuncName(classSymbol, method, FlatCharset::Unicode) + u"_SetOverrideCallback"; }
	ln::String makeWrapFuncName_SignalCaller(const TypeSymbol* classSymbol, const MethodSymbol* method) const { return u"Wrap_" + makeFuncName(classSymbol, method, FlatCharset::Unicode) + u"_SignalCaller"; }

	ln::String makeAccessorCacheDecls(const TypeSymbol* classSymbol) const;
	ln::String makeWrapFuncImplement(const TypeSymbol* classSymbol, const MethodOverloadInfo* overloadInfo) const;
	ln::String makeWrapFuncCallBlock(const TypeSymbol* classSymbol, const MethodSymbol* method) const;
	ln::String makeVALUEReturnExpr(TypeSymbol* type, const ln::String& varName) const;
	ln::String makeTypeCheckExpr(const TypeSymbol* type, const ln::String& varName) const;
	ln::String makeVALUEToNativeCastDecl(const MethodParameterSymbol* param) const;
	ln::String makeConstandValue(const ConstantSymbol* constant) const;

	ln::String makeWrapFuncImplement_SetOverrideCallback(const TypeSymbol* classInfo) const;
	ln::String makeWrapFuncImplement_SignalCaller(const MethodSymbol* method) const;
	ln::String makeWrapFuncImplement_EventConnector(const MethodSymbol* method) const;


	ln::String makeEnumTypeVALUEName(TypeSymbol* type) const { return u"g_enum_" + type->shortName(); }
	ln::String makeEnumTypeVALUEVariableDecls() const;
	ln::String makeEnumTypeRegisterCode() const;

	ln::String makeEventSignalDefinition() const;
};

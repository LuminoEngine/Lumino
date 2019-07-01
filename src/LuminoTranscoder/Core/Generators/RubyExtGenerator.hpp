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
	ln::String makeWrapStructName(TypeSymbol* type) const { return "Wrap_" + type->shortName(); }
	ln::String makeWrapFuncName(MethodSymbol* method) const { return "Wrap_" + FlatCCommon::makeFuncName(config(), method); }
	ln::String makeWrapFuncImplement(MethodOverloadInfo* overloadInfo) const;
	ln::String makeWrapFuncCallBlock(MethodSymbol* method) const;
};

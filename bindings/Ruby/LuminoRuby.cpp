#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

// 関数名は create_makefile で指定した名前と合わせておかなければならない
extern "C" void Init_Lumino()
{
	// ルートモジュール
	g_luminoModule = rb_define_module("Lumino");
	
    // 例外定義
    g_luminoError = rb_define_class_under(g_luminoModule, "LuminoError", rb_eRuntimeError);
	
	InitEnums();
	InitStructs();
	InitClasses();
}



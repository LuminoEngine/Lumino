#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

const char* LNGetLastErrorMessage()
{
	const LNChar* str;
	LNError_GetLastErrorMessage(&str);
	return LNInternal_ConvertToUTF8String(str, -1);
}

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



#include <Common.h>

// 文字列を作成する
DemoMain(Core_String_1, "Core.文字列.文字列を作成する")
{
	String str1 = "Lumino";
}

DemoMain(Core_String_2, "Core.文字列.文字列を結合する")
{
	String str1 = "Lumino";
	str1 += _T("Lumino");
}

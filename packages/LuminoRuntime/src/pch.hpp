#pragma once


// ビルド時間短縮のため、LuminoEngine.dll を作るときは LuminoEngine.lib をリンクして、
// Static-lib 内の関数を Export している。
// ただし、関数の実態定義側に __declspec(dllexport) がないと Export されなかったので、
// Static-lib ビルド時は常に有効にしている。
// #define LUMINO_BUILD_DLL 1

// #define LN_MSVC_DISABLE_LIBRARY_LINK
// #define LN_INTERNAL_ACCESS public
// #define LN_PROTECTED_INTERNAL_ACCESS public



#include <LuminoCore.hpp>

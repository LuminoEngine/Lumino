
#pragma once
#include <LuminoRuntime/Common.hpp>

extern "C"
{









//==============================================================================
// ln::Engine

/**
    @brief エンジンの初期化処理を行います。
*/
LN_API LnResult LnEngine_Initialize();

/**
    @brief エンジンの終了処理を行います。
*/
LN_API LnResult LnEngine_Finalize();

/**
    @brief 1フレーム分の更新処理を行います。
    @return アプリケーションの終了が要求されている場合は false を返します。
    @details この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
*/
LN_API LnResult LnEngine_Update(LnBool* outReturn);



} // extern "C"


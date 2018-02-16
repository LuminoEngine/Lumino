
#pragma once
#include <Lumino/FlatCCommon.h>

extern "C"
{









//==============================================================================
// ln::Engine
//==============================================================================

/**
    @brief エンジンの初期化処理を行います。
*/
LN_API LNResultCode LNEngine_Initialize(LNHandle* outEngine);

/**
    @brief エンジンの終了処理を行います。
*/
LN_API LNResultCode LNEngine_Terminate();

/**
    @brief 1フレーム分の更新処理を行います。
    @return アプリケーションの終了が要求されている場合は false を返します。
    @details この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
*/
LN_API LNResultCode LNEngine_Update(bool* outReturn);

//==============================================================================
// ln::Sound
//==============================================================================

/**
    @brief この音声の音量を設定します。
    @param[in] volume : 音量 (0.0～1.0。初期値は 1.0)
*/
LN_API LNResultCode LNSound_SetVolume(LNHandle sound, float volume);

/**
    @brief この音声の音量を取得します。
*/
LN_API LNResultCode LNSound_GetVolume(LNHandle sound, float* outReturn);

/**
    @brief この音声のピッチ (音高) を設定します。
    @param[in] volume : ピッチ (0.5～2.0。初期値は 1.0)
*/
LN_API LNResultCode LNSound_SetPitch(LNHandle sound, float pitch);

/**
    @brief この音声のピッチ (音高) を取得します。
*/
LN_API LNResultCode LNSound_GetPitch(LNHandle sound, float* outReturn);

/**
    @brief ループ再生の有無を設定します。
    @param[in] enabled : ループ再生するか
*/
LN_API LNResultCode LNSound_SetLoopEnabled(LNHandle sound, bool enabled);

/**
    @brief ループ再生が有効かを確認します。
*/
LN_API LNResultCode LNSound_IsLoopEnabled(LNHandle sound, bool* outReturn);

/**
    @brief ループ範囲を設定します。
    @param[in] begin : ループ範囲の開始サンプル
    @param[in] length : ループ範囲のサンプル数
    @details MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。
*/
LN_API LNResultCode LNSound_SetLoopRange(LNHandle sound, uint32_t begin, uint32_t length);

/**
    @brief この音声の再生を開始します。
*/
LN_API LNResultCode LNSound_Play(LNHandle sound);

/**
    @brief この音声の再生を停止します。
*/
LN_API LNResultCode LNSound_Stop(LNHandle sound);

/**
    @brief この音声の再生を一時停止します。
*/
LN_API LNResultCode LNSound_Pause(LNHandle sound);

/**
    @brief 一時停止中の再生を再開します。
*/
LN_API LNResultCode LNSound_Resume(LNHandle sound);

/**
    @brief Sound クラスのインスタンスを作成します。
*/
LN_API LNResultCode LNSound_Initialize(const LNChar* filePath, LNHandle* outSound);



} // extern "C"


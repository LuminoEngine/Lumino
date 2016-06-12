
#pragma once
#include "LNCommon.h"
#include "LNTypedef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
	@defgroup   group_ui ユーザーインターフェイス機能
	@brief		ユーザーインターフェイス機能
	@{
*/
LN_MODULE(UI)

//==============================================================================
/**
	@brief	フレームウィンドウのベースクラスです。
*/
LN_CLASS(LNUIFrameWindow, LNObject)
	
LN_CLASS_END

//==============================================================================
/**
	@brief	ネイティブウィンドウをホストするフレームウィンドウのクラスです。
*/
LN_CLASS(LNUINativeHostWindow, LNUIFrameWindow)

	/**
		@brief		UINativeHostWindow オブジェクトを作成します。
		@param[in]	windowHandle		: ネイティブウィンドウの識別子 (Windows では HWND)
		@param[out]	outUIFrameWindow	: 作成された フレームウィンドウのハンドル を格納する変数のポインタ
	*/
	LN_INSTANCE_API
	LN_ATTR_CONSTRUCTOR
	LNResult LNUINativeHostWindow_Create(intptr_t windowHandle, LN_OUT LN_HANDLE(LNUINativeHostWindow)* outUIFrameWindow);
	
	/**
		@brief		ウィンドウの内容を描画します。
		@param[in]	frameWindow		: LNUIFrameWindow ハンドル
	*/
	LN_INSTANCE_API
	LNResult LNUINativeHostWindow_Render(LN_HANDLE(LNUINativeHostWindow) frameWindow);

LN_CLASS_END

LN_MODULE_END /** @} */

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus

#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		UI 機能の管理クラスです。
*/
class UI
{
public:
	
	/**
		@brief		全ての UIContext を更新します。
	*/
	static void UpdateAllContext();

	/**
		@brief		全ての UIContext を描画します。
		@details	この関数は Renderer::BeginRendering() と Renderer::EndRendering() の間で呼び出します。
	*/
	static void RenderAllContext();

#if 0
	/**
		@brief		メインウィンドウと関連付けられた UIContext に描画先のピクセルサイズ変更イベントを通知します。
		@param[in]	width		: 幅
		@param[in]	height		: 高さ
	*/
	static bool InjectViewportSizeChanged(int width, int height);

	/**
		@brief		メインウィンドウと関連付けられた UIContext にマウス移動イベントを通知します。
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	static bool InjectMouseMove(float clientX, float clientY);

	/**
		@brief		メインウィンドウと関連付けられた UIContext にマウスボタンが押された時のイベントを通知します。
		@param[in]	button		: 操作されたマウスボタン
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	static bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);

	/**
		@brief		メインウィンドウと関連付けられた UIContext にマウスボタンが離された時のイベントを通知します。
		@param[in]	button		: 操作されたマウスボタン
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	static bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);

	/**
		@brief		メインウィンドウと関連付けられた UIContext にマウスホイールが操作された時のイベントを通知します。
		@param[in]	delta		: ホイールの移動数
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	static bool InjectMouseWheel(int delta, float clientX, float clientY);

	/**
		@brief		メインウィンドウと関連付けられた UIContext にキーが押された時のイベントを通知します。
		@param[in]	keyCode		: キーコード
		@param[in]	isAlt		: Alt キーが押されている場合は true
		@param[in]	isShift		: Shift キーが押されている場合は true
		@param[in]	isControl	: Ctrl キーが押されている場合は true
	*/
	static bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);

	/**
		@brief		メインウィンドウと関連付けられた UIContext にキーが離された時のイベントを通知します。
		@param[in]	keyCode		: キーコード
		@param[in]	isAlt		: Alt キーが押されている場合は true
		@param[in]	isShift		: Shift キーが押されている場合は true
		@param[in]	isControl	: Ctrl キーが押されている場合は true
	*/
	static bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);

	/**
		@brief		メインウィンドウと関連付けられた UIContext に文字が入力された時のイベントを通知します。
		@param[in]	ch			: 入力された文字
	*/
	static bool InjectTextInput(TCHAR ch);

	/**
		@brief		メインウィンドウと関連付けられた UIContext に時間経過を通知します。
		@param[in]	elapsedTime	: 経過時間 (秒)
	*/
	void InjectElapsedTime(float elapsedTime);
#endif
};

LN_NAMESPACE_END

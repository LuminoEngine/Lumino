
#pragma once
#include "../Common.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief	GUI システムへの入力イベントの入り口となるインターフェイスです。
*/
class IUIInjectedInputReceiver
{
public:
	
	/**
		@brief		描画先のピクセルサイズ変更イベントを受け取ります。
		@param[in]	width		: 幅
		@param[in]	height		: 高さ
	*/
	virtual bool InjectViewportSizeChanged(int width, int height) = 0;

	/**
		@brief		マウス移動イベントを受け取ります。
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool InjectMouseMove(float clientX, float clientY) = 0;

	/**
		@brief		マウスボタンが押された時のイベントを受け取ります。
		@param[in]	button		: 操作されたマウスボタン
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY) = 0;

	/**
		@brief		マウスボタンが離された時のイベントを受け取ります。
		@param[in]	button		: 操作されたマウスボタン
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY) = 0;

	/**
		@brief		マウスホイールが操作された時のイベントを受け取ります。
		@param[in]	delta		: ホイールの移動数
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool InjectMouseWheel(int delta, float clientX, float clientY) = 0;

	/**
		@brief		キーが押された時のイベントを受け取ります。
		@param[in]	keyCode		: キーコード
		@param[in]	isAlt		: Alt キーが押されている場合は true
		@param[in]	isShift		: Shift キーが押されている場合は true
		@param[in]	isControl	: Ctrl キーが押されている場合は true
	*/
	virtual bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl) = 0;

	/**
		@brief		キーが離された時のイベントを受け取ります。
		@param[in]	keyCode		: キーコード
		@param[in]	isAlt		: Alt キーが押されている場合は true
		@param[in]	isShift		: Shift キーが押されている場合は true
		@param[in]	isControl	: Ctrl キーが押されている場合は true
	*/
	virtual bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl) = 0;

	/**
		@brief		文字が入力された時のイベントを受け取ります。
		@param[in]	ch			: 入力された文字
	*/
	virtual bool InjectTextInput(TCHAR ch) = 0;

	/**
		@brief		時間経過が通知されます。
		@param[in]	elapsedTime	: 経過時間 (秒)
	*/
	virtual void InjectElapsedTime(float elapsedTime) = 0;

protected:
	virtual ~IUIInjectedInputReceiver() {}
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

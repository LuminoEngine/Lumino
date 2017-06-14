
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

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
	//virtual bool InjectViewportSizeChanged(int width, int height) = 0;

	/**
		@brief		マウス移動イベントを受け取ります。
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool injectMouseMove(float clientX, float clientY) = 0;

	/**
		@brief		マウスボタンが押された時のイベントを受け取ります。
		@param[in]	button		: 操作されたマウスボタン
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool injectMouseButtonDown(MouseButtons button, float clientX, float clientY) = 0;

	/**
		@brief		マウスボタンが離された時のイベントを受け取ります。
		@param[in]	button		: 操作されたマウスボタン
		@param[in]	clientX		: X 座標
		@param[in]	clientY		: Y 座標
	*/
	virtual bool injectMouseButtonUp(MouseButtons button, float clientX, float clientY) = 0;

	/**
		@brief		マウスホイールが操作された時のイベントを受け取ります。
		@param[in]	delta		: ホイールの移動数
	*/
	virtual bool injectMouseWheel(int delta) = 0;

	/**
		@brief		キーが押された時のイベントを受け取ります。
		@param[in]	keyCode		: キーコード
		@param[in]	modifierKeys: 修飾キー
	*/
	virtual bool injectKeyDown(Keys keyCode, ModifierKeys modifierKeys) = 0;

	/**
		@brief		キーが離された時のイベントを受け取ります。
		@param[in]	keyCode		: キーコード
		@param[in]	modifierKeys: 修飾キー
	*/
	virtual bool injectKeyUp(Keys keyCode, ModifierKeys modifierKeys) = 0;

	/**
		@brief		文字が入力された時のイベントを受け取ります。
		@param[in]	ch			: 入力された文字
	*/
	virtual bool injectTextInput(TCHAR ch) = 0;

protected:
	virtual ~IUIInjectedInputReceiver() {}
};

LN_NAMESPACE_END


#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Platform/Window.h>
//#include "../../include/Lumino/EventListener.h"

namespace Lumino
{
namespace Platform
{

#if 0
/**
	@brief	プラットフォーム固有のウィンドウシステムのウィンドウを抽象化します。
*/
class NativeWindow
	: public RefObject
{
public:

	/**
		@brief		クライアント領域のサイズを取得します。
	*/
	virtual const Size& GetSize() const = 0;
	
	/**
		@brief		フルスクリーンの有効フラグを設定します。
		@details	enabled に true を設定するとフルスクリーン、false を設定するとウィンドウモードになります。
					この関数はウィンドウスタイルを変更して最大化するだけです。
					ユーザー定義ウィンドウの場合はなにもしません。
	*/
	virtual void SetFullScreenEnabled(bool enabled) = 0;

	/**
		@brief		フルスクリーン状態かを判定します。
	*/
	virtual bool IsFullScreenEnabled() const = 0;

	/**
		@brief		ウィンドウがアクティブかどうかを示す値を取得します。
	*/
	virtual bool IsActive() const = 0;

	/**
		@brief		このウィンドウにマウスキャプチャを設定します。
	*/
	virtual void CaptureMouse() = 0;

	/**
		@brief		このウィンドウからマウスキャプチャを削除します。
	*/
	virtual void ReleaseMouseCapture() = 0;

	/**
		@brief		このウィンドウにイベントリスナーをアタッチします。(priority が大きいものが先に処理される。必ずデタッチすること)
	*/
	virtual void AttachEventListener(IEventListener* listener, int priority) = 0;

	/**
		@brief		このウィンドウからイベントリスナーをデタッチします。
	*/
	virtual void DetachEventListener(IEventListener* listener) = 0;

protected:
	virtual ~NativeWindow() {};
};
#endif

} // namespace Platform
} // namespace Lumino




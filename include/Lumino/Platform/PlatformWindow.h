
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/SortedArray.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include "Common.h"
#include "EventListener.h"

LN_NAMESPACE_BEGIN

/**
	@brief	プラットフォーム固有のウィンドウシステムのウィンドウを抽象化します。
*/
class PlatformWindow
	: public tr::ReflectionObject
{
public:

	/**
		@brief		ウィンドウを作成します。
		@param[in]	title		: ウィンドウタイトル
		@param[in]	size		: クライアント領域のピクセルサイズ
		@param[in]	resizable	: 可変ウィンドウとして作成するかどうか
	*/
	static PlatformWindow* Create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager);

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
		@brief		マウスカーソルがクライアント領域上にある場合に表示するかを設定します。
	*/
	virtual void SetCursorVisible(bool visible);

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
	void AttachEventListener(IEventListener* listener, int priority);

	/**
		@brief		このウィンドウからイベントリスナーをデタッチします。
	*/
	void DetachEventListener(IEventListener* listener);

protected:
	PlatformWindow(WindowManagerBase* windowManager);
	virtual ~PlatformWindow();

	typedef SortedArray<int, IEventListener*>	EventListenerList;

	WindowManagerBase*		m_windowManager;
	EventListenerList		m_listenerEntryArray;

LN_INTERNAL_ACCESS:
	bool SendEventToAllListener(const PlatformEventArgs& e);

	detail::MouseCursorVisibility* m_mouseCursorVisibility;
};

LN_NAMESPACE_END




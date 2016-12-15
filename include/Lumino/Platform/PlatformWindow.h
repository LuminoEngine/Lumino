
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/SortedArray.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include "../Base/GeometryStructs.h"
#include "Common.h"
#include "EventListener.h"

LN_NAMESPACE_BEGIN

/**
	@brief	プラットフォーム固有のウィンドウシステムのウィンドウを抽象化します。
*/
class PlatformWindow
	: public RefObject
{
public:

	/**
		@brief		ウィンドウを作成します。
		@param[in]	title		: ウィンドウタイトル
		@param[in]	size		: クライアント領域のピクセルサイズ
		@param[in]	resizable	: 可変ウィンドウとして作成するかどうか
	*/
	//static PlatformWindow* Create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager);

public:

	/**
		@brief		クライアント領域のサイズを取得します。
	*/
	SizeI GetSize() const { return m_clientSize; }

	virtual void SetSize(const SizeI& size) = 0;

	virtual void SetVisible(bool visible) = 0;

	virtual void SetTitleText(const StringRef& title) = 0;
	virtual const String& GetTitleText() const = 0;
	
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
	bool IsActive() const { return m_isActive; }

	/**
		@brief		マウスカーソルがクライアント領域上にある場合に表示するかを設定します。
	*/
	virtual void SetCursorVisible(bool visible);

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
	void Initialize(const SizeI& clientSize);
    
LN_INTERNAL_ACCESS:
	bool SendPlatformEvent(const PlatformEventArgs& e);
	bool SendPlatformClosingEvent(PlatformWindow* sender);
	bool SendPlatformWindowSizeChangedEvent(int width, int height);
	bool SendPlatformActivateChangedEvent(PlatformWindow* sender, bool active);
	bool SendPlatformKeyEvent(PlatformEventType type_, PlatformWindow* sender_, Key keyCode_, ModifierKeys modifierKeys_, char keyChar_);
	bool SendPlatformMouseWheelEvent(int delta);
	virtual void OnPlatformEvent(const PlatformEventArgs& e);

	typedef SortedArray<int, IEventListener*>	EventListenerList;

	WindowManagerBase*		m_windowManager;
	EventListenerList		m_listenerEntryArray;

LN_INTERNAL_ACCESS:
	bool SendEventToAllListener(const PlatformEventArgs& e);

	detail::MouseCursorVisibility* m_mouseCursorVisibility;

private:
	SizeI	m_clientSize;
	bool	m_isActive;
};

LN_NAMESPACE_END




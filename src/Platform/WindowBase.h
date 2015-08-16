
#pragma once

#include "NativeWindow.h"
#include "MouseCursorVisibility.h"

namespace Lumino
{
namespace Platform
{
class WindowManagerBase;

/**
	@brief	
*/
class WindowBase		// TODO: 無理に Base と Window に分けると複雑になる。GUI みたいに Helper を使うようにしたい。
	: public Window
{
public:
	WindowBase(WindowManagerBase* windowManager);
	virtual ~WindowBase();

public:
	void SetCursorVisible(bool visible) { m_mouseCursorVisibility.SetMouseCursorVisibleState(visible, 0); }
	//void SetApplication(Win32WindowManager* app) { m_windowManager = app; }

	/// このウィンドウに割り当てられている全てのイベントリスナーにイベントを送信する
	/// (ウィンドウシステムに送信するのではない点に注意)
	bool SendEventToAllListener(const EventArgs& e);

public:
	// override Window
	virtual void AttachEventListener(IEventListener* listener, int priority) { m_listenerEntryArray.Add(priority, listener); }
	virtual void DetachEventListener(IEventListener* listener) { return m_listenerEntryArray.RemoveAllValue(listener); }

protected:
	typedef SortedArray<int, IEventListener*>	EventListenerList;

	WindowManagerBase*		m_windowManager;	///< (循環参照防止のため、参照カウントは操作しない)
	EventListenerList		m_listenerEntryArray;
	MouseCursorVisibility	m_mouseCursorVisibility;

};

} // namespace Platform
} // namespace Lumino

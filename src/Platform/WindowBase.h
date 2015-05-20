
#pragma once

#include "NativeWindow.h"

namespace Lumino
{
namespace Platform
{
class WindowManagerBase;

/**
	@brief	
*/
class WindowBase
	: public Window
{
public:
	WindowBase(WindowManagerBase* windowManager);
	virtual ~WindowBase();

public:
	//void SetApplication(Win32WindowManager* app) { m_windowManager = app; }

	/// このウィンドウに割り当てられている全てのイベントリスナーにイベントを送信する
	/// (ウィンドウシステムに送信するのではない点に注意)
	bool SendEventToAllListener(const EventArgs& e);

public:
	// override Window
	virtual void AttachEventListener(IEventListener* listener, int priority) { m_listenerEntryArray.Add(priority, listener); }
	virtual void DetachEventListener(IEventListener* listener) { return m_listenerEntryArray.Remove(listener); }

protected:
	WindowManagerBase*					m_windowManager;	///< (循環参照防止のため、参照カウントは操作しない)
	SortedArray<int, IEventListener*>	m_listenerEntryArray;
};

} // namespace Platform
} // namespace Lumino

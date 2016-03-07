
#include "../Internal.h"
#include <Lumino/Platform/Window.h>
#include <Lumino/Platform/PlatformManager.h>
#include "WindowManagerBase.h"
#include "MouseCursorVisibility.h"

LN_NAMESPACE_BEGIN
namespace Platform
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window* Window::Create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager)
{
	WindowCreationSettings data;
	data.Title = title;
	data.ClientSize = clientSize;
	data.Fullscreen = false;
	data.Resizable = resizable;
	return manager->m_windowManager->CreateSubWindow(data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window::Window(WindowManagerBase* windowManager)
	: m_windowManager(windowManager)
	, m_mouseCursorVisibility(LN_NEW detail::MouseCursorVisibility)
{
	/*	Window は Graphics や Input、GUI 等、さまざまなところから参照される。
		また、WinAPI の UnregisterClass() は全てのウィンドウを DestroyWindow() してから呼ばなければならない。
		(これも assert や例外として返ってくるので原因がわかりにくかった・・・。)
		これまでは WindowManager に Finalize() を実装してデストラクタの前に色々解放処理をしていたが、これはアウト。
		全ての Window が解放された後で WindowManager の解放処理が走るように、ここで参照カウントを増やしておく。
	*/
	LN_SAFE_ADDREF(m_windowManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window::~Window()
{
	LN_SAFE_DELETE(m_mouseCursorVisibility);
	LN_SAFE_RELEASE(m_windowManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::SetCursorVisible(bool visible)
{
	m_mouseCursorVisibility->SetMouseCursorVisibleState(visible, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::AttachEventListener(IEventListener* listener, int priority)
{
	m_listenerEntryArray.Add(priority, listener);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::DetachEventListener(IEventListener* listener)
{
	m_listenerEntryArray.RemoveAllValue(listener);
}

//-----------------------------------------------------------------------------
// このウィンドウに割り当てられている全てのイベントリスナーにイベントを送信する
// (ウィンドウシステムに送信するのではない点に注意)
//-----------------------------------------------------------------------------
bool Window::SendEventToAllListener(const EventArgs& e)
{
	for (const EventListenerList::Pair& listener : m_listenerEntryArray)
	{
		if (listener.second->OnEvent(e)) {
			return true;
		}
	}
	return false;
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window::Window(NativeWindow* nativeWindow)
	: m_nativeWindow(nativeWindow)
{
	LN_SAFE_ADDREF(m_nativeWindow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window::Window(const String& title, const Size& clientSize, bool resizable)
{
	WindowCreationSettings data;
	data.Title = title;
	data.ClientSize = clientSize;
	data.Fullscreen = false;
	data.Resizable = resizable;
	m_nativeWindow = Internal::ApplicationInstance->m_windowManager->CreateSubWindow(data);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Window::~Window()
{
	LN_SAFE_RELEASE(m_nativeWindow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Window::GetSize() const
{
	return m_nativeWindow->GetSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::SetFullScreenEnabled(bool enabled)
{
	m_nativeWindow->SetFullScreenEnabled(enabled);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Window::IsFullScreenEnabled() const
{
	return m_nativeWindow->IsFullScreenEnabled();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Window::IsActive() const
{
	return m_nativeWindow->IsActive();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::CaptureMouse()
{
	m_nativeWindow->CaptureMouse();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::ReleaseMouseCapture()
{
	m_nativeWindow->ReleaseMouseCapture();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::AttachEventListener(IEventListener* listener, int priority)
{
	m_nativeWindow->AttachEventListener(listener, priority);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Window::DetachEventListener(IEventListener* listener)
{
	m_nativeWindow->DetachEventListener(listener);
}
#endif

} // namespace Platform
LN_NAMESPACE_END




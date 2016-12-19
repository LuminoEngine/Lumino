
#include "../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include "PlatformManager.h"
#include "PlatformWindowManagerBase.h"
#include "MouseCursorVisibility.h"

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
//PlatformWindow* PlatformWindow::Create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager)
//{
//	WindowCreationSettings data;
//	data.title = title;
//	data.clientSize = clientSize;
//	data.fullscreen = false;
//	data.resizable = resizable;
//	return manager->m_windowManager->CreateSubWindow(data);
//}

//------------------------------------------------------------------------------
PlatformWindow::PlatformWindow(WindowManagerBase* windowManager)
	: m_windowManager(windowManager)
	, m_mouseCursorVisibility(LN_NEW detail::MouseCursorVisibility)
	, m_isActive(true)	// 初期値 true。WM_ACTIVATE は初回表示で最前面になった時は呼ばれない TODO: Win32 だけ？
{
	/*	Window は Graphics や Input、GUI 等、さまざまなところから参照される。
		また、WinAPI の UnregisterClass() は全てのウィンドウを DestroyWindow() してから呼ばなければならない。
		(これも assert や例外として返ってくるので原因がわかりにくかった・・・。)
		これまでは WindowManager に Finalize() を実装してデストラクタの前に色々解放処理をしていたが、これはアウト。
		全ての PlatformWindow が解放された後で WindowManager の解放処理が走るように、ここで参照カウントを増やしておく。
	*/
	LN_SAFE_ADDREF(m_windowManager);
}

//------------------------------------------------------------------------------
PlatformWindow::~PlatformWindow()
{
	LN_SAFE_DELETE(m_mouseCursorVisibility);
	LN_SAFE_RELEASE(m_windowManager);
}

//------------------------------------------------------------------------------
void PlatformWindow::Initialize(const SizeI& clientSize)
{
	m_clientSize = clientSize;
}

//------------------------------------------------------------------------------
void PlatformWindow::SetCursorVisible(bool visible)
{
	m_mouseCursorVisibility->SetMouseCursorVisibleState(visible, 0);
}

//------------------------------------------------------------------------------
void PlatformWindow::AttachEventListener(IEventListener* listener, int priority)
{
	m_listenerEntryArray.Add(priority, listener);
}

//------------------------------------------------------------------------------
void PlatformWindow::DetachEventListener(IEventListener* listener)
{
	m_listenerEntryArray.RemoveAllValue(listener);
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformEvent(const PlatformEventArgs& e)
{
	OnPlatformEvent(e);
	return SendEventToAllListener(e);
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformClosingEvent(PlatformWindow* sender)
{
	PlatformEventArgs e(PlatformEventType::Close, this);

	if (SendPlatformEvent(e)) {
		return true;
	}

	// TODO
	if (this == m_windowManager->GetMainWindow()) {
		m_windowManager->Exit();
	}
	return true;
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformWindowSizeChangedEvent(int width, int height)
{
	// ウィンドウサイズを拾っておく
	m_clientSize.Set(width, height);

	PlatformEventArgs e(PlatformEventType::WindowSizeChanged, this);
	e.size.width = width;
	e.size.height = height;
	SendPlatformEvent(e);
	return true;
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformActivateChangedEvent(PlatformWindow* sender, bool active)
{
	if (active != m_isActive)
	{
		m_isActive = active;

		PlatformEventArgs e;
		e.type = (active) ? PlatformEventType::WindowActivate : PlatformEventType::WindowDeactivate;
		e.sender = sender;
		return SendPlatformEvent(e);
	}
	return false;
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformKeyEvent(PlatformEventType type_, PlatformWindow* sender_, Keys keyCode_, ModifierKeys modifierKeys_, char keyChar_)
{
	PlatformEventArgs e;
	e.type = type_;
	e.sender = sender_;
	e.key.keyCode = keyCode_;
	e.key.modifierKeys = (ModifierKeys::value_type)modifierKeys_.GetValue();
	e.key.keyChar = keyChar_;
	return SendPlatformEvent(e);
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformMouseWheelEvent(int delta)
{
	PlatformEventArgs e;
	e.type = PlatformEventType::MouseWheel;
	e.sender = this;
	e.wheel.delta = delta;
	return SendPlatformEvent(e);
}

//------------------------------------------------------------------------------
void PlatformWindow::OnPlatformEvent(const PlatformEventArgs& e)
{
}

//------------------------------------------------------------------------------
// このウィンドウに割り当てられている全てのイベントリスナーにイベントを送信する
// (ウィンドウシステムに送信するのではない点に注意)
//------------------------------------------------------------------------------
bool PlatformWindow::SendEventToAllListener(const PlatformEventArgs& e)
{
	for (const EventListenerList::Pair& listener : m_listenerEntryArray)
	{
		if (listener.second->OnEvent(e)) {
			return true;
		}
	}
	return false;
}

LN_NAMESPACE_END




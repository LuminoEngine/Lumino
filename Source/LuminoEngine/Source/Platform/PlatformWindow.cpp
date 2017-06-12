
#include "../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include "PlatformManager.h"
#include "PlatformWindowManagerBase.h"
#include "MouseCursorVisibility.h"

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
//PlatformWindow* PlatformWindow::create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager)
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
void PlatformWindow::initialize(const SizeI& clientSize)
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
	m_listenerEntryArray.add({ priority, listener });
	std::stable_sort(
		m_listenerEntryArray.begin(), m_listenerEntryArray.end(),
		[](const std::pair<int, IEventListener*>& lhs, const std::pair<int, IEventListener*>& rhs) { return lhs.first < rhs.first; });
}

//------------------------------------------------------------------------------
void PlatformWindow::DetachEventListener(IEventListener* listener)
{
	m_listenerEntryArray.removeIf(
		[listener](const std::pair<int, IEventListener*>& i) { return i.second == listener; });
}

//------------------------------------------------------------------------------
bool PlatformWindow::SendPlatformEvent(const PlatformEventArgs& e)
{
	OnPlatformEvent(e);
	bool handled = SendEventToAllListener(e);
	if (!handled)
	{
		// 以下、デフォルト処理

		switch (e.type)
		{
			case PlatformEventType::close:
			{
				// TODO
				if (this == m_windowManager->GetMainWindow())
				{
					m_windowManager->Exit();
					return true;
				}
				break;
			}
			case PlatformEventType::WindowSizeChanged:
			{
				// ウィンドウサイズを拾っておく
				m_clientSize.set(e.size.width, e.size.height);
				break;
			}
			case PlatformEventType::WindowActivate:
			{
				m_isActive = true;
				break;
			}
			case PlatformEventType::WindowDeactivate:
			{
				m_isActive = false;
				break;
			}

			default:
				break;
		}
	}

	return handled;
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
	for (auto& listener : m_listenerEntryArray)
	{
		if (listener.second->OnEvent(e)) {
			return true;
		}
	}
	return false;
}

LN_NAMESPACE_END




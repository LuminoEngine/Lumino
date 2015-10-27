
#include "../Internal.h"
#include "WindowManagerBase.h"
#include "WindowBase.h"

LN_NAMESPACE_BEGIN
namespace Platform
{

//=============================================================================
// WindowBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WindowBase::WindowBase(WindowManagerBase* windowManager)
	: m_windowManager(windowManager)
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
WindowBase::~WindowBase()
{
	LN_SAFE_RELEASE(m_windowManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool WindowBase::SendEventToAllListener(const EventArgs& e)
{
	for (const EventListenerList::Pair& listener: m_listenerEntryArray)
	{
		if (listener.second->OnEvent(e)) {
			return true;
		}
	}
	return false;
}

} // namespace Platform
LN_NAMESPACE_END

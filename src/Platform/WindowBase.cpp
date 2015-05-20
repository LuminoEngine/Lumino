
#include "../Internal.h"
#include "WindowBase.h"

namespace Lumino
{
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
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
WindowBase::~WindowBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool WindowBase::SendEventToAllListener(const EventArgs& e)
{
	LN_FOREACH(IEventListener* listener, m_listenerEntryArray)
	{
		if (listener->OnEvent(e)) {
			return true;
		}
	}
	return false;
}

} // namespace Platform
} // namespace Lumino
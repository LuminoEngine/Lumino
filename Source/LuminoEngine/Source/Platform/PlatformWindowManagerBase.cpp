
#include "../Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include "PlatformWindowManagerBase.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// WindowManagerBase
//==============================================================================
WindowManagerBase::WindowManagerBase()
	: m_endRequested(false)
	, m_dragDropData()
{
	m_dragDropData.data = newObject<DataObject>();
	m_dragDropData.effect = DragDropEffects::None;
}

LN_NAMESPACE_END

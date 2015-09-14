
#include "../Internal.h"
#include <Lumino/GUI/GUIContext.h>
#include "GUIManagerImpl.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
	
//=============================================================================
// GUIContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext* GUIContext::Create()
{
	return LN_NEW GUIContext(GUIManagerImpl::Instance);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext::GUIContext(GUIManagerImpl* manager)
	: m_manager(NULL)
{
	LN_REFOBJ_SET(m_manager, manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext::~GUIContext()
{
	LN_SAFE_RELEASE(m_manager);
}

LN_NAMESPACE_GUI_END
} // namespace Lumino

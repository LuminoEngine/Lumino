
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/GUIContext.h>
#include "GUIManagerImpl.h"
#include "GUIHelper.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
	
//=============================================================================
// GUIContext
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(GUIContext, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext* GUIContext::Create()
{
	RefPtr<GUIContext> obj(LN_NEW GUIContext(GUIManagerImpl::Instance));
	GUIManagerImpl::Instance->AddContextOnMainWindow(obj);
	obj->m_onMainWindow = true;
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext::GUIContext(GUIManagerImpl* manager)
	: m_manager(NULL)
	, m_rootElement(NULL)
	, m_viewPixelSize()
	, m_onMainWindow(false)
{
	LN_REFOBJ_SET(m_manager, manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext::~GUIContext()
{
	LN_SAFE_RELEASE(m_rootElement);
	if (m_onMainWindow) {
		m_manager->RemoveContextOnMainWindow(this);
	}
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::SetRootElement(UIElement* element)
{
	if (m_rootElement != NULL && element == NULL) {
		GUIHelper::DetachContext(m_rootElement);
	}
	LN_REFOBJ_SET(m_rootElement, element);
	if (m_rootElement != NULL) {
		GUIHelper::AttachContext(m_rootElement, this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* GUIContext::GetRootElement() const
{
	return m_rootElement;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& GUIContext::GetViewPixelSize() const
{
	return m_viewPixelSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::UpdateLayout(const Size& viewSize)
{
	m_viewPixelSize = viewSize;	// この後の UIElement::UpdateLayout() で参照される

	if (m_rootElement != NULL)
	{
		m_rootElement->ApplyTemplate();
		m_rootElement->UpdateLayout();
		m_rootElement->UpdateTransformHierarchy();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::Render()
{
	if (m_rootElement != NULL) {
		m_rootElement->Render();
	}
}

LN_NAMESPACE_GUI_END
} // namespace Lumino

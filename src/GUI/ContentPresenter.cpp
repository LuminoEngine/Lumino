
#include "../Internal.h"
#include <Lumino/GUI/ContentPresenter.h>

namespace Lumino
{
namespace GUI
{
	
//=============================================================================
// ContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentPresenter, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenterPtr ContentPresenter::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::ContentPresenter(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::~ContentPresenter()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::SetContent(UIElement* content)
{
	if (m_content.GetObjectPtr() == content) {
		return;
	}

	// 先に古いのは取り除く
	if (m_content != NULL) {
		m_content->SetInheritanceParent(NULL);
		m_content->SetParent(NULL);
	}

	m_content = content;
	m_content->SetInheritanceParent(this);
	m_content->SetParent(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ContentPresenter::GetVisualChildrenCount() const
{
	if (m_content != NULL) {
		return UIElement::GetVisualChildrenCount() + 1;
	}
	return UIElement::GetVisualChildrenCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* ContentPresenter::GetVisualChild(int index) const
{
	if (index < UIElement::GetVisualChildrenCount()) {
		return UIElement::GetVisualChild(index);
	}
	return m_content;
}

#if 1
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ContentPresenter::ApplyTemplateHierarchy(CombinedLocalResource* parent)
//{
//	UIElement::ApplyTemplateHierarchy(parent);
//	if (m_content != NULL) {
//		m_content->ApplyTemplateHierarchy(m_combinedLocalResource);
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ContentPresenter::MeasureOverride(const SizeF& constraint)
{
	if (m_content != NULL)
	{
		m_content->MeasureLayout(constraint);
		return m_content->GetDesiredSize();
	}
	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ContentPresenter::ArrangeOverride(const SizeF& finalSize)
{
	if (m_content != NULL) {
		m_content->ArrangeLayout(RectF(0, 0, finalSize));
	}
	return finalSize;
}


#endif


} // namespace GUI
} // namespace Lumino

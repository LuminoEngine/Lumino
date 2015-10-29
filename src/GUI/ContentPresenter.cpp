
#include "Internal.h"
#include <Lumino/GUI/ContentControl.h>
#include <Lumino/GUI/ContentPresenter.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN
	
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
	return ContentPresenterPtr(internalCreateInstance(GetUIManager()), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::ContentPresenter(GUIManagerImpl* manager)
	: UIElement(manager)
	, m_owner(NULL)
	, m_content()
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
		m_content->ChangeContext(NULL);
		m_content->SetParent(NULL);
	}

	m_content = content;
	m_content->SetInheritanceParent(this);
	m_content->SetParent(this);
	m_content->SetTemplateModified(true);	// テンプレートを再構築する必要がありそう
	m_content->ChangeContext(GetContext());	// この中でビジュアルツリーの再構築が行われる。Parent は決まっていなければならない。
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
	if (m_content != NULL)
	{
		RectF childRect;
		const SizeF& ds = m_content->GetDesiredSize();

		// 水平方向
		HorizontalAlignment ha = m_owner->GetHorizontalContentAlignment();
		switch (ha)
		{
		case HorizontalAlignment::Left:
			childRect.X = 0;
			childRect.Width = ds.Width;
			break;
		case HorizontalAlignment::Center:
			childRect.X = (finalSize.Width - ds.Width) / 2;
			childRect.Width = ds.Width;
			break;
		case HorizontalAlignment::Right:
			childRect.X = finalSize.Width - ds.Width;
			childRect.Width = ds.Width;
			break;
		case HorizontalAlignment::Stretch:
			childRect.X = 0;
			childRect.Width = finalSize.Width;
			break;
		}
		// 垂直方向
		VerticalAlignment va = m_owner->GetVerticalContentAlignment();
		switch (va)
		{
		case VerticalAlignment::Top:
			childRect.Y = 0;
			childRect.Height = ds.Height;
			break;
		case VerticalAlignment::Center:
			childRect.Y = (finalSize.Height - ds.Height) / 2;
			childRect.Height = ds.Height;
			break;
		case VerticalAlignment::Bottom:
			childRect.Y = finalSize.Height - ds.Height;
			childRect.Height = ds.Height;
			break;
		case VerticalAlignment::Stretch:
			childRect.Y = 0;
			childRect.Height = finalSize.Height;
			break;
		}

		m_content->ArrangeLayout(childRect);
	}
	return finalSize;
}


#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::SetOwner(ContentControl* owner)
{
	assert(m_owner == NULL);
	m_owner = owner;
}


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

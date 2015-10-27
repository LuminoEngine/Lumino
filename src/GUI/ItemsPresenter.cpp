
#include "Internal.h"
#include <Lumino/GUI/ItemsPresenter.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// ItemsPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsPresenter, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenterPtr ItemsPresenter::Create()
{
	return ItemsPresenterPtr(internalCreateInstance(GetUIManager()), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenter::ItemsPresenter(GUIManagerImpl* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenter::~ItemsPresenter()
{

}

//-----------------------------------------------------------------------------
// この関数はオーナーの ItemsControl が「この Panel でレイアウトしろ」と通知するためのもの。
// この ItemsPresenter が オーナーの ItemsControl に割り当てられたときや、
// オーナーの ItemsControl の HostPanelTemplate が変わったときに呼ばれる。
//-----------------------------------------------------------------------------
void ItemsPresenter::AttachHostPanel(Panel* panel)
{
	// 古いのは取り除く
	if (m_hostPanel != NULL)
	{
		RemoveVisualChild(m_hostPanel);
	}

	// 新しいのを追加する
	m_hostPanel = panel;
	if (m_hostPanel != NULL)
	{
		AddVisualChild(m_hostPanel);
		m_hostPanel->ApplyTemplate();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ItemsPresenter::MeasureOverride(const SizeF& constraint)
{
	//MeasureElementWithSingleChild

	SizeF size = UIElement::MeasureOverride(constraint);
	if (m_hostPanel != NULL)
	{
		m_hostPanel->MeasureLayout(constraint);
		SizeF childSize = m_hostPanel->GetDesiredSize();
		size.Width = std::max(size.Width, childSize.Width);
		size.Height = std::max(size.Height, childSize.Height);
	}
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ItemsPresenter::ArrangeOverride(const SizeF& finalSize)
{
	SizeF size = UIElement::ArrangeOverride(finalSize);
	if (m_hostPanel != NULL)
	{
		RectF rect(0, 0, size);
		m_hostPanel->ArrangeLayout(rect);
	}
	return size;
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END


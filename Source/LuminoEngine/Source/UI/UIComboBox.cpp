
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIScrollViewer.h>
#include <Lumino/UI/UIComboBox.h>
#include <Lumino/UI/UILayoutPanel.h>	// TODO: stackpanel にするかも
#include <Lumino/UI/UILayoutView.h>
#include "UIManager.h"
#include "UIHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIPopup
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIPopup, UIContentControl)

//------------------------------------------------------------------------------
UIPopup::UIPopup()
{
}

//------------------------------------------------------------------------------
UIPopup::~UIPopup()
{
}

//------------------------------------------------------------------------------
void UIPopup::initialize()
{
	UIElement::initialize();
}

//------------------------------------------------------------------------------
void UIPopup::SetContent(UIElement* element)
{
	if (m_content == element) return;

	m_content = element;

	// 既に持っていれば取り除いておく
	if (m_content != nullptr)
	{
		RemoveVisualChild(m_content);
		m_content = nullptr;
	}

	// 新しく保持する
	if (element != nullptr)
	{
		AddVisualChild(element);
		m_content = element;
	}
}

//------------------------------------------------------------------------------
void UIPopup::open()
{
	UIElement* root = UIHelper::GetLayoutRoot(this);
	if (root != nullptr)
	{
		UILayoutView* rootView = static_cast<UILayoutView*>(root);
		rootView->OpenPopup(this);
	}
}

//------------------------------------------------------------------------------
Size UIPopup::measureOverride(const Size& constraint)
{
	// Popup は常にサイズ 0 となる。
	// また、子要素のレイアウトは行わない。
	// 子要素のレイアウトを行うのは別途、Popup 専用のレイアウトフェーズ。
	return Size();
}

//------------------------------------------------------------------------------
Size UIPopup::arrangeOverride(const Size& finalSize)
{
	return UIElement::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UIPopup::UpdateLayoutForInPlacePopup(const Size& viewSize)
{
	m_content->UpdateLayout(viewSize);
	//m_content->measureLayout(viewSize);

	////TODO: このへんでchildの位置を決める

	//m_content->arrangeLayout(RectF(0, 0, viewSize));
	//m_content->updateTransformHierarchy(RectF(0, 0, viewSize));
}


//==============================================================================
// UIComboBoxItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIComboBoxItem, UIContentControl)

//------------------------------------------------------------------------------
UIComboBoxItem::UIComboBoxItem()
{
}

//------------------------------------------------------------------------------
UIComboBoxItem::~UIComboBoxItem()
{
}

//------------------------------------------------------------------------------
void UIComboBoxItem::initialize()
{
	UIContentControl::initialize();
	SetHContentAlignment(HAlignment::Left);
	SetHAlignment(HAlignment::Stretch);
	GoToVisualState(UIVisualStates::NormalState);
}

//==============================================================================
// UIComboBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIComboBox, UIControl)


//------------------------------------------------------------------------------
UIComboBoxPtr UIComboBox::create()
{
	auto ptr = UIComboBoxPtr::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIComboBox::UIComboBox()
{
}

//------------------------------------------------------------------------------
UIComboBox::~UIComboBox()
{
}

//------------------------------------------------------------------------------
void UIComboBox::initialize()
{
	UIControl::initialize();

	m_popup = newObject<UIPopup>();
	AddVisualChild(m_popup);

	m_scrollViewer = newObject<UIScrollViewer>();
	m_scrollViewer->SetWidth(100);	// TODO:
	m_scrollViewer->SetHeight(30);
	m_scrollViewer->SetBackground(Brush::Blue);
	m_popup->SetContent(m_scrollViewer);

	SetHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::makeRef();
	panel->initialize();
	panel->SetHAlignment(HAlignment::Stretch);
	panel->SetVAlignment(VAlignment::Stretch);
	SetLayoutPanel(panel);
	GoToVisualState(UIVisualStates::NormalState);
}

//------------------------------------------------------------------------------
//UIComboBoxItemPtr UIComboBox::AddTextItem(const String& text)
//{
//	auto textBlock = RefPtr<UITextBlock>::MakeRef();
//	textBlock->initialize(getManager());
//	textBlock->SetText(text);
//	return AddItem(textBlock);
//}

//------------------------------------------------------------------------------
//UIComboBoxItemPtr UIComboBox::AddItem(UIElement* item)
//{
//	if (LN_CHECK_ARG(item != nullptr)) return nullptr;
//
//	// 受け取った item を UIComboBoxItem でラップして、UIComboBoxItem をリストに入れる
//	auto listItem = RefPtr<UIComboBoxItem>::MakeRef();
//	listItem->initialize(getManager());
//	listItem->SetContent(item);
//	GetItems()->Add(listItem);
//	return listItem;
//}

//------------------------------------------------------------------------------
void UIComboBox::OnMouseDown(UIMouseEventArgs* e)
{
	m_popup->open();
	UIControl::OnMouseDown(e);
}

LN_NAMESPACE_END

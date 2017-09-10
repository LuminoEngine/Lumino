
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIScrollViewer.h>
#include <Lumino/UI/UIComboBox.h>
#include <Lumino/UI/UILayoutPanel.h>	// TODO: stackpanel にするかも
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIInput.h>
#include "UIManager.h"
#include "UIHelper.h"
#include "LayoutHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIAdorner
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIAdorner, UIElement)

UIAdorner::UIAdorner()
{
}

UIAdorner::~UIAdorner()
{
}

void UIAdorner::initialize()
{
	UIElement::initialize();
}

//==============================================================================
// UIAdornerLayer
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIAdornerLayer, UIElement)

UIAdornerLayer::UIAdornerLayer()
{
}

UIAdornerLayer::~UIAdornerLayer()
{
}

void UIAdornerLayer::initialize()
{
	UIElement::initialize();
	setHitTestVisible(false);	// TODO: デフォルト false がいいだろう。control は true で。
	writeCoreFlag(detail::UICoreFlags_AdornerLayer, true);
}

void UIAdornerLayer::add(UIAdorner* adorner)
{
	adorner->setLogicalParent(this);
	addVisualChild(adorner);
	m_adorners.add(adorner);
}

void UIAdornerLayer::remove(UIAdorner* adorner)
{
	if (LN_REQUIRE(adorner->getLogicalParent() == this)) return;
	adorner->setLogicalParent(nullptr);
	removeVisualChild(adorner);
	m_adorners.remove(adorner);
}

Size UIAdornerLayer::measureOverride(const Size& constraint)
{
	// LayoutPanel ではなく、UIElement の measureOverride を実施 (this のサイズを測る)
	return detail::LayoutHelper2::measureOverride_AbsoluteLayout<UIAdornerLayer, UIElement, List<Ref<UIAdorner>>>(
		this, constraint, m_adorners);
}

Size UIAdornerLayer::arrangeOverride(const Size& finalSize)
{
	return detail::LayoutHelper2::arrangeOverride_AbsoluteLayout<UIAdornerLayer, UIElement, List<Ref<UIAdorner>>>(
		this, finalSize, m_adorners);
}

//==============================================================================
// UIPopup
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIPopup, UIContentControl)
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIPopup, UIAdorner)

//------------------------------------------------------------------------------
UIPopup::UIPopup()
	: m_layoutView(nullptr)
{
}

//------------------------------------------------------------------------------
UIPopup::~UIPopup()
{
}

//------------------------------------------------------------------------------
void UIPopup::initialize()
{
	UIAdorner::initialize();
	setHitTestVisible(true);
	setFocusable(true);
}

//------------------------------------------------------------------------------
void UIPopup::setContent(UIElement* element)
{
	if (m_content == element) return;

	m_content = element;

	// 既に持っていれば取り除いておく
	if (m_content != nullptr)
	{
		removeVisualChild(m_content);
		m_content = nullptr;
	}

	// 新しく保持する
	if (element != nullptr)
	{
		addVisualChild(element);
		m_content = element;
	}
}

//------------------------------------------------------------------------------
void UIPopup::open(UIElement* owner)
{
	if (LN_REQUIRE(m_layoutView == nullptr)) return;

	auto* parent = getVisualParent();
	if (parent == nullptr) parent = owner;

	UIElement* root = UIHelper::getLayoutRoot(parent);
	if (root != nullptr)
	{
		m_layoutView = static_cast<UILayoutView*>(root);
		m_layoutView->openPopup(this);
		setPosition(UIMouse::getPosition(m_layoutView));	// TODO: WPF だとこの辺に PlacementMode がかかってくる
		focus();
	}
}

void UIPopup::close()
{
	// TODO: WPF は StaysOpen で制御できる
	if (m_layoutView != nullptr)
	{
		m_layoutView->closePopup(this);
		m_layoutView = nullptr;
	}
}

//------------------------------------------------------------------------------
Size UIPopup::measureOverride(const Size& constraint)
{
	// Popup は常にサイズ 0 となる。
	// また、子要素のレイアウトは行わない。
	// 子要素のレイアウトを行うのは別途、Popup 専用のレイアウトフェーズ。
	//return Size();
	//return UIElement::measureOverride(constraint);

	return detail::LayoutHelper2::measureOverride_SimpleOneChild<UIPopup, UIAdorner>(this, constraint, m_content);
}

//------------------------------------------------------------------------------
Size UIPopup::arrangeOverride(const Size& finalSize)
{
	//return UIElement::arrangeOverride(finalSize);
	return detail::LayoutHelper2::arrangeOverride_SimpleOneChild<UIPopup, UIAdorner>(this, finalSize, m_content);
}

void UIPopup::onGotFocus(UIEventArgs* e)
{
	UIAdorner::onGotFocus(e);
}

void UIPopup::onLostFocus(UIEventArgs* e)
{
	close();
	UIAdorner::onLostFocus(e);
}

//------------------------------------------------------------------------------
void UIPopup::updateLayoutForInPlacePopup(const Size& viewSize)
{
	updateLayout(viewSize);

	//if (m_content != nullptr)
	//{
	//	m_content->updateLayout(viewSize);
	//}
	//m_content->measureLayout(viewSize);

	////TODO: このへんでchildの位置を決める

	//m_content->arrangeLayout(RectF(0, 0, viewSize));
	//m_content->updateTransformHierarchy(RectF(0, 0, viewSize));
}


//==============================================================================
// UIComboBoxItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIComboBoxItem, UIControl)

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
	UIControl::initialize();
	setHContentAlignment(HAlignment::Left);
	setHAlignment(HAlignment::Stretch);
	goToVisualState(UIVisualStates::NormalState);
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
	addVisualChild(m_popup);

	m_scrollViewer = newObject<UIScrollViewer>();
	m_scrollViewer->setWidth(100);	// TODO:
	m_scrollViewer->setHeight(30);
	m_scrollViewer->setBackground(Brush::Blue);
	m_popup->setContent(m_scrollViewer);

	setHContentAlignment(HAlignment::Stretch);

	auto panel = Ref<UIStackPanel>::makeRef();
	panel->initialize();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	setLayoutPanel(panel);
	goToVisualState(UIVisualStates::NormalState);
}

//------------------------------------------------------------------------------
//UIComboBoxItemPtr UIComboBox::addTextItem(const String& text)
//{
//	auto textBlock = Ref<UITextBlock>::MakeRef();
//	textBlock->initialize(getManager());
//	textBlock->SetText(text);
//	return AddItem(textBlock);
//}

//------------------------------------------------------------------------------
//UIComboBoxItemPtr UIComboBox::AddItem(UIElement* item)
//{
//	if (LN_REQUIRE(item != nullptr)) return nullptr;
//
//	// 受け取った item を UIComboBoxItem でラップして、UIComboBoxItem をリストに入れる
//	auto listItem = Ref<UIComboBoxItem>::MakeRef();
//	listItem->initialize(getManager());
//	listItem->setContent(item);
//	getItems()->Add(listItem);
//	return listItem;
//}

//------------------------------------------------------------------------------
void UIComboBox::onMouseDown(UIMouseEventArgs* e)
{
	m_popup->open(this);
	UIControl::onMouseDown(e);
}

LN_NAMESPACE_END

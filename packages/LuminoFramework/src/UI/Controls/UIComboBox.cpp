#include "Internal.hpp"
#include <LuminoFramework/UI/UIRenderingContext.hpp>
#include <LuminoFramework/UI/Layout/UILayoutPanel.hpp>
#include <LuminoFramework/UI/UIPopup.hpp>
#include <LuminoFramework/UI/Controls/UIComboBox.hpp>

namespace ln {

//==============================================================================
// UIComboBoxItem

UIComboBoxItem::UIComboBoxItem()
	: m_ownerComboBox(nullptr)
{
	m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

bool UIComboBoxItem::init()
{
	if (!UIControl::init()) return false;

	static int cc = 0;
	setBackgroundColor((cc == 0) ? Color::Red : Color::Blue);
	cc++;

	return true;
}

void UIComboBoxItem::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
		e->handled = true;

		if (m_ownerComboBox) {
			m_ownerComboBox->selectItem(this);
		}
		//raiseEvent(UIEventArgs::create(this, UIEvents::ComboBoxItemClicked));

		return;
	}

	UIControl::onRoutedEvent(e);
}

//==============================================================================
// UIComboBox

/*

[2020/7/22] UIComboBox.Items を持つべきか？UIComboBoxItem.Parent を作るべきか？
----------
あえて持たせないようにすると、UIComboBoxItem は親 UIComboBox がどこにあるかを気にせず自由にレイアウトすることができるようになる。
UIComboBox への addItem/addChild は不要。

ユーザプログラムはちょっとシンプルになるが、実装はかなり難易度高くなるので止めた方がいいかも…。
UIComboBox というよりその中の ListBox の実装だが、
- 要素を列挙するような操作が難しくなる。
- ↓キーで「次のItem」を選択するときの検索処理がすごく重くなる。まず 親 UIComboBox を探して、次にその子をすべて走査して、選択中の「次」を調べないとならない。
- カスタマイズが難しくなる。やっぱり論理的な親子関係を明示的に持っていてほしい。聞けばなるほどだけど、驚きの方が多いか。

[2020/7/18] カスタマイズについて考察
----------
ListBox も同じような感じであるが…

例えば、Blender みたいなグループ化されたわりと自由なアイテム配置を実現したいことがある。
ユーザーが完全に制御するには、
- ComboBoxItem クラスを触れるようにしないとだめ
- ItemsHostPanel は Canvas(自由配置) を使う

ComboBoxItem クラスは m_ownerCollectionControl とか持たずに、ComboBoxItemClickedEvent を Bubble で上げれば？とか思ったけど、
UIComboBox::items() みたいに UIComboBox の API として全項目を列挙したいときに面倒になる。
やっぱり UIComboBox は List で UIComboBoxItem を持ってないといろいろ大変。複数選択とかもやりづらいか。

今は UIComboBoxItem は addChild 時に内部で作ってたりするけど、そうすると UIComboBoxItem の派生を作れなくなる。
TreeView みたいに onGenerateItem を実装すれば？とか思ったけど、~View 系と ~Box 系は用途を明確に分ける方針にしたのでこれはやりたくない。
~Box 系は Item を細かく制御でき、柔軟なレイアウトを実現するのに使いたい。

なので大方針は、
- addChild(String) -> 内部で UIText と UIComboBoxItem を作り、UIComboBoxItem は UIText を Content として持つ。
- addChild(UIElement) -> 内部で UIComboBoxItem を作り、UIComboBoxItem は指定された UIElement を Content として持つ。
- addChild(UIComboBoxItem) -> 指定された UIComboBoxItem をそのまま Item として使う。

カスタマイズは
```
combobox->setItemsHostPanel(nullptr);
auto item = UIComboBoxItem::create();
combobox->addChild(item);
item->好きなように移動したり色変えたりする。
```

レイアウトをネストする場合
```
combobox->setItemsHostPanel(nullptr);	// これは null
auto item = UIComboBoxItem::create();
combobox->addChild(layout);	// layout を直接の子にする
combobox->addItem(item);	// Item としての管理だけ追加する。Visual及びLogicalの子要素としては追加しない。
item->好きなように移動したり色変えたりする。
```

*/

UIComboBox::UIComboBox()
{
}

void UIComboBox::init()
{
    UIControl::init();
	m_autoLayoutLogicalChildren = false;
	//m_layoutItemsHostLayoutEnabled = false;

	auto itemsHost = makeObject_deprecated<UIStackLayout>();
	itemsHost->setOrientation(UILayoutOrientation::Vertical);
	m_itemsHost = itemsHost;
	//setItemsLayoutPanel(m_itemsHost, false);

	// TODO: test
	//m_itemsHost->setBackgroundColor(Color::BlueViolet);
	m_itemsHost->setMargin(5);

    m_popup = ln::makeObject_deprecated<ln::UIPopup>();
    //m_popup->setWidth(200);
    //m_popup->setHeight(300);
    //m_popup->setBackgroundColor(Color::AliceBlue);
    m_popup->setPlacementTarget(this);
	m_popup->addElement(m_itemsHost);
    addVisualChild(m_popup);


	// TODO: test
	//addChild(u"test1");
	//addChild(u"test2");


	// dummy for single select mode
	m_selectedItems.add(nullptr);
}

void UIComboBox::onAddChild(UIElement* child)
{
    auto item = ln::makeObject_deprecated<UIComboBoxItem>();
	item->m_ownerComboBox = this;
    item->addElement(child);


	m_logicalChildren->add(item);
	item->setLogicalParent(this);

	if (m_itemsHost) {
		m_itemsHost->addVisualChild(item);
	}

	/*
		Note: UIContorl が LogicalChildren に対して Layout を実行してしまっているため、UIComboBoxItem が Adorner からではなく UIComboBox からレイアウトされてしまっている。
	*/

    //UIControl::addItem(item);
}

void UIComboBox::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::MouseDownEvent) {
        if (m_popup->isOpend()) {
            m_popup->close();
        }
        else {
            m_popup->open();
        }
        e->handled = true;
        return;
    }
    else if (e->type() == UIEvents::MouseUpEvent) {
		e->handled = true;
        return;
    }
	//else if (e->type() == UIEvents::ComboBoxItemClicked) {

	//	selectItem(static_cast<UIComboBoxItem*>(e->sender()));

	//	e->handled = true;
	//	return;
	//}

	UIControl::onRoutedEvent(e);
}

//Size UIComboBox::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
//{
//	if (m_itemsHost) {
//		m_itemsHost->measureLayout(layoutContext, constraint);
//		Size layoutSize = m_itemsHost->desiredSize();
//		Size localSize = UIElement::measureOverride(layoutContext, constraint);
//		return Size::max(layoutSize, localSize);
//	}
//	else {
//		return UIControl::measureOverride(layoutContext, constraint);
//	}
//}
//
//Size UIComboBox::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
//{
//	Rect contentSlotRect = detail::LayoutHelper::makePaddingRect(this, finalSize);
//
//	if (m_itemsHost) {
//		m_itemsHost->arrangeLayout(layoutContext, contentSlotRect);
//		return finalSize;
//	}
//	else {
//		return UIControl::arrangeOverride(layoutContext, finalSize);
//	}
//}
//
void UIComboBox::onUpdateLayout(UILayoutContext* layoutContext)
{
	UIControl::onUpdateLayout(layoutContext);
	m_popup->setWidth(actualSize().width);
}

void UIComboBox::onRender(UIRenderingContext* context)
{
	UIControl::onRender(context);


	auto area = detail::LayoutHelper::makePaddingRect(this, actualSize());


	auto item = selectedItem();
	if (item) {

		// vertical centering.
		float y = (area.height - item->actualSize().height) / 2;
		auto offset = Matrix::makeTranslation(area.x, area.y + y, 0);

		for (auto& child : item->m_visualChildren) {
			context->drawVisual(child, offset);
		}
	}
}

void UIComboBox::selectItem(UIComboBoxItem* item)
{
	//if (m_selectedItems[0]) {
	//	m_selectedItems[0]->setSelectedInternal(false);
	//}

	m_selectedItems[0] = item;
	//item->setSelectedInternal(true);

	//onSelectionChanged(UISelectionChangedEventArgs::create(this, UIEvents::SelectionChanged));
	
	m_popup->close();
}

//void UIComboBox::onItemClick(UICollectionItem* item, UIClickEventArgs* e)
//{
//	UIItemsControl::onItemClick(item, e);
//	m_popup->close();
//}

} // namespace ln


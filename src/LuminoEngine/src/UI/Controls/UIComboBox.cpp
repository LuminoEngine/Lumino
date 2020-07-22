
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIPopup.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>

namespace ln {

//==============================================================================
// UIComboBoxItem

UIComboBoxItem::UIComboBoxItem()
{
}

void UIComboBoxItem::init()
{
    UICollectionItem::init();

	static int cc = 0;
	setBackgroundColor((cc == 0) ? Color::Red : Color::Blue);
	cc++;
}

//==============================================================================
// UIComboBox

/*
[2020/7/28] カスタマイズについて考察
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
- addChild(String) -> 内部で UITextBlock と UIComboBoxItem を作り、UIComboBoxItem は UITextBlock を Content として持つ。
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
    UIItemsControl::init();
	m_layoutItemsHostLayoutEnabled = false;

	auto itemsHost = makeObject<UIStackLayout2_Obsolete>();
	itemsHost->setOrientation(Orientation::Vertical);
	m_itemsHost = itemsHost;
	setItemsLayoutPanel(m_itemsHost, false);

	// TODO: test
	//m_itemsHost->setBackgroundColor(Color::BlueViolet);
	m_itemsHost->setMargin(5);

    m_popup = ln::makeObject<ln::UIPopup>();
    //m_popup->setWidth(200);
    //m_popup->setHeight(300);
    //m_popup->setBackgroundColor(Color::AliceBlue);
    m_popup->setPlacementTarget(this);
	m_popup->addElement(m_itemsHost);
    addVisualChild(m_popup);


	// TODO: test
	//addChild(u"test1");
	//addChild(u"test2");
}

void UIComboBox::onAddChild(UIElement* child)
{
    auto item = ln::makeObject<UIComboBoxItem>();
    item->addElement(child);

    UIItemsControl::addItem(item);
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
        return;
    }

	UIItemsControl::onRoutedEvent(e);
}

void UIComboBox::onUpdateLayout(UILayoutContext* layoutContext)
{
	UIItemsControl::onUpdateLayout(layoutContext);
	m_popup->setWidth(actualSize().width);
}

void UIComboBox::onRender(UIRenderingContext* context)
{
	UIItemsControl::onRender(context);


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

void UIComboBox::onItemClick(UICollectionItem* item, UIClickEventArgs* e)
{
	UIItemsControl::onItemClick(item, e);
	m_popup->close();
}

} // namespace ln


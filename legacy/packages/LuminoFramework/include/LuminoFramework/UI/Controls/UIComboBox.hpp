#pragma once
#include "../UIItemsElement.hpp"

namespace ln {
class UIPopup;
class UILayoutPanel;
class UIComboBox;

class UIComboBoxItem
    : public UIControl
{
public:
    virtual const String& elementName() const  override { static String name = _TT("UIComboBoxItem"); return name; }
    // TODO: group


    bool onHitTest(const Point& frameClientPosition) override
    {
        bool r = UIControl::onHitTest(frameClientPosition);
        if (r) {
            UIControl::onHitTest(frameClientPosition);
            printf("");
        }
        return r;
    }

protected:
    void onRoutedEvent(UIEventArgs* e) override;
    Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override
    {
        return UIControl::arrangeOverride(layoutContext, finalArea);
    }
    void onRender(UIRenderingContext* context) override
    {
        UIControl::onRender(context);
    }

LN_CONSTRUCT_ACCESS:
    UIComboBoxItem();
    bool init();

private:
    UIComboBox* m_ownerComboBox;

    friend class UIComboBox;
};

class UIComboBox
	: public UIControl
{
public:
    UIComboBoxItem* selectedItem() const { return (m_selectedItems.isEmpty()) ? nullptr :  m_selectedItems[0]; }

protected:
    virtual const String& elementName() const  override { static String name = _TT("UIComboBox"); return name; }
    virtual void onAddChild(UIElement* child) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;
    //virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    //virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
	virtual void onUpdateLayout(UILayoutContext* layoutContext);
	virtual void onRender(UIRenderingContext* context) override;

	//virtual void onItemClick(UICollectionItem* item, UIClickEventArgs* e);

LN_CONSTRUCT_ACCESS:
    UIComboBox();
	void init();

private:
    void selectItem(UIComboBoxItem* item);

    Ref<UIPopup> m_popup;
	Ref<UILayoutPanel> m_itemsHost;
    List<UIComboBoxItem*> m_selectedItems;

    friend class UIComboBoxItem;
};

} // namespace ln


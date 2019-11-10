#pragma once
#include "UIContainerElement.hpp"
#include "UIAdorner.hpp"

namespace ln {
class UIDialogAdorner;

class UIDialog
    : public UIContainerElement
{
public:
    bool isOpend() const { return m_opend; }

    void open();
    void close();

protected:
    virtual const String& elementName() const  override { static String name = u"UIDialog"; return name; }

LN_CONSTRUCT_ACCESS:
	UIDialog();
    void init();

private:
    Ref<UIAdorner> m_adorner;
    bool m_opend;

    friend class UIDialogAdorner;
};

// Backdrop も兼ねる
class UIDialogAdorner
    : public UIAdorner
{
protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
    virtual void onUpdateLayout(UILayoutContext* layoutContext, const Rect& finalGlobalRect) override;
	virtual UIElement* lookupMouseHoverElement(const Point& frameClientPosition) override;
    virtual void render(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIDialogAdorner();
    void init(UIDialog* popup);

private:
    Ref<UIDialog> m_popup;
};

//class UIBackdrop
//	: public UIElement
//{
//public:
//	
//LN_CONSTRUCT_ACCESS:
//	UIBackdrop();
//    void init();
//
//private:
//};

} // namespace ln


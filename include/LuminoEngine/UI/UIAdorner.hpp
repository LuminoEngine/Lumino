#pragma once
#include "UIElement.hpp"

namespace ln {


class UIAdorner
    : public UIElement
{
public:
    UIElement* adornedElement() const;
    void setContent(UIElement* element);
    UIElement* content() const;

    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;
    virtual void render(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    UIAdorner();
    void init(UIElement* adornedElement);

private:
    Ref<UIElement> m_adornedElement;
    Ref<UIElement> m_content;
};

class UIAdornerLayer
    : public Object
{
public:
    void add(UIAdorner* adorner);
    void remove(UIAdorner* adorner);

    void measureLayout(const Size& availableSize);
    void arrangeLayout(const Rect& localSlotRect);
    void render(UIRenderingContext* context);

LN_CONSTRUCT_ACCESS:
    UIAdornerLayer();
    void init();

private:
    List<Ref<UIAdorner>> m_adorners;
};

} // namespace ln


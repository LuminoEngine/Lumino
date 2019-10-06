#pragma once
#include "UIElement.hpp"

namespace ln {

// Adorner-Layer 上に配置される UIElement をホストするための要素
class UIAdorner
    : public UIElement
{
public:
    UIElement* adornedElement() const;

LN_CONSTRUCT_ACCESS:
    UIAdorner();
    void init(UIElement* adornedElement);

private:
    Ref<UIElement> m_adornedElement;
};

class UIAdornerLayer
    : public Object
{
public:
    void add(UIAdorner* adorner);
    void remove(UIAdorner* adorner);

    void updateStyleHierarchical(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle);
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


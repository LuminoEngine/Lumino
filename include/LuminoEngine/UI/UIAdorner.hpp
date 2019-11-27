#pragma once
#include "UIElement.hpp"

namespace ln {
class UIFrameRenderView;

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
    void measureLayout(UILayoutContext* layoutContext, const Size& availableSize);
    void arrangeLayout(UILayoutContext* layoutContext, const Rect& localSlotRect);
    void render(UIRenderingContext* context, const Matrix& parentTransform);
	UIElement* lookupMouseHoverElement(const Point& frameClientPosition);

LN_CONSTRUCT_ACCESS:
    UIAdornerLayer();
    void init(UIFrameRenderView* owner);

private:
    List<Ref<UIAdorner>> m_adorners;
    UIFrameRenderView* m_owner;
};

} // namespace ln


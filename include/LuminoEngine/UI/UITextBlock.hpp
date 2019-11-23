#pragma once
#include "UIElement.hpp"

namespace ln {
class Material;
class RTDocument;
//namespace detail {
//class FlexText;
//}

class UITextBlock
    : public UIElement
{
    LN_OBJECT;
public:
    static Ref<UITextBlock> create();
    static Ref<UITextBlock> create(const StringRef& text);

    void setText(const StringRef& value) { m_text = value; }

LN_CONSTRUCT_ACCESS:
    UITextBlock();
	void init(); 
    void init(const StringRef& text);

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual void onRender(UIRenderingContext* context) override;

private:
    String m_text;
};

} // namespace ln


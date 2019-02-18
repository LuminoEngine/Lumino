#pragma once
#include "UIElement.hpp"

namespace ln {
class Material;

class UITextBlock
    : public UIElement
{
public:
    static Ref<UITextBlock> create();

    void setText(const StringRef& value) { m_text = value; }

LN_CONSTRUCT_ACCESS:
    UITextBlock();
	void init();

protected:
    virtual void onRender(UIRenderingContext* context);

private:
    String m_text;
};

} // namespace ln


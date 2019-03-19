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
	virtual Size measureOverride(const Size& constraint) override;
    virtual void onRender(UIRenderingContext* context) override;

private:
    String m_text;
};

} // namespace ln


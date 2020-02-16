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

	void setText(const StringRef& value);
	const String& text() const { return m_text; }


	ViewProperty* getViewProperty(StringRef name);
	std::vector<Ref<ViewProperty>> m_viewProperties;

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


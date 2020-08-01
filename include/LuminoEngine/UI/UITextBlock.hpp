#pragma once
#include "UIElement.hpp"

namespace ln {
class RTDocument;
//namespace detail {
//class FlexText;
//}

/**
 * 文字列を表示するための UI 要素です。少量の文字列表示に最適化されています。
 */
LN_CLASS()
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

//LN_CONSTRUCT_ACCESS:
public: // TODO:
    UITextBlock();
    
    /**
     * UITextBlock を作成します。
     */
    LN_METHOD()
	void init();

    /**
     * 表示文字列を指定して、UITextBlock を作成します。
     * @param[in] text : 表示文字列
     */
    LN_METHOD(OverloadPostfix = "WithText")
    void init(const StringRef& text);

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual void onRender(UIRenderingContext* context) override;


    bool onHitTest(const Point& frameClientPosition) override
    {
        bool r = UIElement::onHitTest(frameClientPosition);
        if (r) {
            UIElement::onHitTest(frameClientPosition);
            printf("");
        }
        return r;
    }


private:
    String m_text;
};

} // namespace ln


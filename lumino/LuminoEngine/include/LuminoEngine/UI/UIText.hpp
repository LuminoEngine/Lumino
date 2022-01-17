#pragma once
#include "UIElement.hpp"

namespace ln {
class RTDocument;

/**
 * 文字列を表示するための UI 要素です。少量の文字列表示に最適化されています。
 */
LN_CLASS()
class UIText
    : public UIElement
{
    LN_OBJECT;
    LN_BUILDER;
public:
    static Ref<UIText> create();
    static Ref<UIText> create(const StringView& text);

    /** 表示文字列を設定します。 */
    LN_METHOD(Property)
	void setText(const StringView& value);

    /** 表示文字列を取得します。 */
    LN_METHOD(Property)
	const String& text() const { return m_text; }


	ViewProperty* getViewProperty(StringView name);
	std::vector<Ref<ViewProperty>> m_viewProperties;

//LN_CONSTRUCT_ACCESS:
public: // TODO:
    UIText();
    
    /**
     * UIText を作成します。
     */
    LN_METHOD()
	void init();

    /**
     * 表示文字列を指定して、UITextBlock を作成します。
     * @param[in] text : 表示文字列
     */
    LN_METHOD(OverloadPostfix = "WithText")
    void init(const StringView& text);

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
    Size m_textSize;
};

//==============================================================================
// UIText::Builder

struct UIText::BuilderDetails : public UIElement::BuilderDetails
{
    LN_BUILDER_DETAILS(UIText);

    String text;

    void apply(UIText* p) const;
};

template<class T, class B, class D>
struct UIText::BuilderCore : public UIElement::BuilderCore<T, B, D>
{
    LN_BUILDER_CORE(UIElement::BuilderCore);

    B& text(StringView value) { d()->text = value; return self(); }
};

struct UIText::Builder : public BuilderCore<UIText, Builder, BuilderDetails>
{
    Builder() {}
    Builder(StringView text) { d()->text = text; }
};

} // namespace ln


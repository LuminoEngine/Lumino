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
	virtual Size measureOverride(const Size& constraint) override;
    virtual void onRender(UIRenderingContext* context) override;

private:
    String m_text;
};


// - typing
// - transform/animation par glyph
class UIMessageTextArea
	: public UIElement
{
public:
	static Ref<UIMessageTextArea> create();

	//void setText(const StringRef& value) { m_text = value; }

	void setTypingSpeed(float value) { m_typingSpeed = value; }

LN_CONSTRUCT_ACCESS:
    UIMessageTextArea();
	void init();

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
	Ref<RTDocument> m_document;
	//Ref<detail::FlexText> m_flexText;
	float m_typingSpeed;
};

} // namespace ln


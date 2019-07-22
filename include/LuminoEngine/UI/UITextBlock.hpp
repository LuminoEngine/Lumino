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


class UITypographyArea
	: public UIElement
{
public:
	static Ref<UITypographyArea> create();

	//void setText(const StringRef& value) { m_text = value; }

	void setTypingSpeed(float value) { m_typingSpeed = value; }

LN_CONSTRUCT_ACCESS:
	UITypographyArea();
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


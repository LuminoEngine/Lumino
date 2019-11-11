#pragma once
#include "UIElement.hpp"

namespace ln {
class RTDocument;

// - typing
// - transform/animation par glyph
// - textlog
// - line feed
// - page feed
// - line centering
class UIMessageTextArea
	: public UIElement
{
public:
	static Ref<UIMessageTextArea> create();

    void setText(const StringRef& value);

	void setTypingSpeed(float value) { m_typingSpeed = value; }

LN_CONSTRUCT_ACCESS:
    UIMessageTextArea();
	void init();

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
	virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
	Ref<RTDocument> m_document;
	//Ref<detail::FlexText> m_flexText;
	float m_typingSpeed;
    String m_text;
    bool m_textDirty;
};

} // namespace ln


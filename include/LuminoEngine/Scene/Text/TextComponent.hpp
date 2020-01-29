
#pragma once
#include "../../Visual/VisualComponent.hpp"

namespace ln {

LN_CLASS()
class TextComponent
	: public VisualComponent
{
	LN_OBJECT;
public:
	/** 表示する文字列を設定します。 */
	LN_METHOD(Property)
	void setText(StringRef value) { m_text = value; }

	/** 表示する文字列を取得します。 */
	LN_METHOD(Property)
	const String& text() const { return m_text; }

	/** テキストの色を設定します。(default: Color::Gray) */
	LN_METHOD(Property)
	void setColor(const Color& value) { m_color = value; }

	/** テキストの色を取得します。 */
	LN_METHOD(Property)
	const Color& color() const { return m_color; }

    /** アンカーポイント (原点) を設定します。デフォルトは中心 (0.5, 0.5) です。 */
	LN_METHOD(Property)
    void setAnchorPoint(const Vector2& value) { m_anchorPoint = value; }

    /** アンカーポイント (原点) を取得します。 */
	LN_METHOD(Property)
    const Vector2& anchorPoint() const { return m_anchorPoint; }

protected:
    virtual void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	TextComponent();
	virtual ~TextComponent() = default;
	bool init();

private:
	String m_text;
	Color m_color;
    Vector2 m_anchorPoint;
};

} // namespace ln

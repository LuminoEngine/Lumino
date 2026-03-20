
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class TextComponent;

/**
 * 
 */
LN_CLASS()
class Text
	: public VisualObject
{
	LN_OBJECT;
public:
	/** Text オブジェクトを作成します。 */
	static Ref<Text> create();

	/** 表示文字列を指定して Text オブジェクトを作成します。 */
	static Ref<Text> create(StringView text);

public:
	/** 表示する文字列を設定します。 */
	LN_METHOD(Property)
	void setText(StringView value);

	/** 表示する文字列を取得します。 */
	LN_METHOD(Property)
	const String& text() const;
	
	/** 表示する文字列を設定します。 (default: nullptr) */
	LN_METHOD(Property)
	void setFont(Font* value);

	/** 表示する文字列を取得します。 (default: nullptr) */
	LN_METHOD(Property)
	Font* font() const;
	
	/** フォントサイズを設定します。 (default: Font::DefaultSize) */
	LN_METHOD(Property)
	void setFontSize(int value);

	/** フォントサイズを取得します。 (default:  Font::DefaultSize) */
	LN_METHOD(Property)
	int fontSize() const;

	/** テキストの色を設定します。(default: Color::Black) */
	LN_METHOD(Property)
	void setColor(const Color& value);

	/** テキストの色を取得します。(default: Color::Black) */
	LN_METHOD(Property)
	const Color& color() const;

    /** アンカーポイント (原点) を設定します。デフォルトは中心 (0.5, 0.5) です。 */
	LN_METHOD(Property)
    void setAnchorPoint(const Vector2& value);

    /** アンカーポイント (原点) を取得します。 */
	LN_METHOD(Property)
    const Vector2& anchorPoint() const ;

LN_CONSTRUCT_ACCESS:
	Text();
	virtual ~Text() = default;

    /** @copydoc create() */
    LN_METHOD()
	bool init();

	/** @copydoc create(StringView) */
	LN_METHOD()
	bool init(StringView text);

private:
    Ref<TextComponent> m_component;
};

} // namespace ln

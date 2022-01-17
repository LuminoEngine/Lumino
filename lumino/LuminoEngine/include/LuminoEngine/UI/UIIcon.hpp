#pragma once
#include "UIElement.hpp"

namespace ln {

/**
 * アイコンの UI 要素です。
 *
 * フォントアイコンを使用する場合、要素のサイズは フォントの Global metrix の行間サイズをベースとする正方形となります。
 * グリフの境界ボックスではありません。
 * このためグリフが異なっていても、フォントサイズが同一であれば UIIcon としてのサイズは同一となります。
 */
LN_CLASS()
class UIIcon
	: public UIElement
{
	LN_OBJECT;
public:
	/** アイコン名を指定して UIIcon を作成します。 */
	LN_METHOD()
	static Ref<UIIcon> loadFontIcon(const StringView& iconName);

	/** アイコン名・サイズ を指定して UIIcon を作成します。 */
	LN_METHOD(OverloadPostfix = "WithNameSize")
	static Ref<UIIcon> loadFontIcon(const StringView& iconName, int size);

	/** アイコン名・サイズ・色 (TextColor) を指定して UIIcon を作成します。 */
	LN_METHOD(OverloadPostfix = "WithNameSizeColor")
	static Ref<UIIcon> loadFontIcon(const StringView& iconName, int size, const Color& color);

	void setIconName(const StringView& value);

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual void onRender(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIIcon();
	bool init();
	bool init(const StringView& iconName);
	bool init(const StringView& iconName, int size);
	bool init(const StringView& iconName, int size, const Color& color);

private:
	Ref<Font> m_font;
	uint32_t m_codePoint;
	Vector2 m_renderOffset;
};

} // namespace ln


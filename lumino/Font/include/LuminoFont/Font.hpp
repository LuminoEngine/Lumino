
#pragma once
#include <LuminoEngine/Reflection/Object.hpp>
#include "Common.hpp"

namespace ln {
namespace detail {
class FontManager;
class FontCore;
class FontHelper;
class FontRequester;
}

/*
 * フォントのクラスです。
 */
class Font
    : public Object
{
public:
	static const int DefaultSize;

    /** デフォルトのスタイルに基づいてフォントを作成します。 */
    static Ref<Font> create();

    /** 指定したフォントファミリとポイントサイズに基づいてフォントを作成します。 */
    static Ref<Font> create(const String& family, float size);

    /** デフォルトのフォントを設定します。 */
    static void setDefaultFont(Font* font);

	/** フォントを登録します。以降、ファミリ名でフォントを使用できるようになります。 */
	static void registerFontFromFile(const StringView& fontFilePath, bool defaultFamily = true);

public:
    /** フォントファミリ名の設定。空文字列はデフォルトのフォントファミリを示します。 */
    void setFamily(const String& familyName);

    /** フォントファミリ名の取得 */
    const String& family() const;

    /** ポイントサイズの有効設定 */
    void setSize(int size);

    /** ポイントサイズの取得 */
    int size() const;

    /** 太文字の有効設定 */
    void setBold(bool enabled);

    /** 太文字の判定 */
    bool isBold() const;

    /** イタリック体の有効設定 */
    void setItalic(bool enabled);

    /** イタリック体の判定 */
    bool isItalic() const;

    /** アンチエイリアスの有効設定 */
    void setAntiAlias(bool enabled);

    /** アンチエイリアスの有効判定 */
    bool isAntiAlias() const;

    /** このフォントのコピーを作成します。*/
    Ref<Font> clone() const;

    /** 指定した文字列を描画する際のピクセルサイズを計算します。*/
    Size measureRenderSize(const StringView& text, float dpiScale = 1.0f);
	Size measureRenderSize(uint32_t codePoint, float dpiScale = 1.0f);

LN_CONSTRUCT_ACCESS:
    Font();
    virtual ~Font();
    void init();
    void init(const String& family, float size);
	void init(const detail::FontDesc& desc);
    //void init(detail::GraphicsManager* manager, FontCore* builtinRawFont);

private:
    detail::FontCore* resolveFontCore(float dpiScale);
	//detail::FontCore* resolveFontCore(const detail::FontRequester& fontRequester);

    detail::FontManager* m_manager;
    detail::FontDesc m_desc;
    Ref<detail::FontCore> m_rawFont;
    bool m_builtin;

    friend class detail::FontHelper;
};

namespace detail {
class FontHelper
{
public:
    static detail::FontCore* resolveFontCore(Font* font, float dpiScale) { return font->resolveFontCore(dpiScale); }
	//static detail::FontCore* resolveFontCore(const FontRequester& fontRequester) { return fontRequester.font->resolveFontCore(fontRequester); }
	static bool equalsFontDesc(const Font* font, const FontDesc& desc) { return font->m_desc.equals(desc); }
    static const FontDesc& getFontDesc(const Font* font) { return font->m_desc; }
};

// TODO: 今の Font を FontFace, これを Font クラスにしてもいいかも。
class FontRequester
	: public RefObject
{
public:
	FontRequester();

	Ref<Font> font;
	int size;
	bool isBold = false;
	bool isItalic = false;
	bool isAntiAlias = false;

	detail::FontCore* resolveFontCore(float scaleFactor);

private:
	detail::FontManager* m_manager;
	Ref<detail::FontCore> m_rawFont;
	float m_lastScaleFactor;
};

} // namespace detail
} // namespace ln

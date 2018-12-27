
#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class FontManager;
class FontCore;
class FontHelper;
}

class Font
    : public Object
{
public:
    /** デフォルトのスタイルに基づいてフォントを作成します。 */
    static Ref<Font> create();

    /** 指定したフォントファミリとポイントサイズに基づいてフォントを作成します。 */
    static Ref<Font> create(const String& family, float size);

    /** デフォルトのフォントを設定します。 */
    static void setDefaultFont(Font* font);

	/** フォントを登録します。以降、ファミリ名でフォントを使用できるようになります。 */
	static void registerFontFile(const StringRef& fontFilePath);

public:
    /** フォントファミリ名の設定 */
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
    Size measureRenderSize(const StringRef& text);

LN_CONSTRUCT_ACCESS:
    Font();
    virtual ~Font();
    void initialize();
    void initialize(const String& family, float size);
    //void initialize(detail::GraphicsManager* manager, FontCore* builtinRawFont);

private:
    detail::FontCore* resolveFontCore();

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
    static detail::FontCore* resolveFontCore(Font* font) { return font->resolveFontCore(); }
};
} // namespace detail
} // namespace ln

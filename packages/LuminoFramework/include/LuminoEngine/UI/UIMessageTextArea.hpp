#pragma once
#include "UIElement.hpp"
#include "Controls/UIWindow.hpp"

namespace ln {

namespace detail { class RTDocument; }

enum class MessageTextPageFeedCursorPosition
{
    BottomCenter,
    EndOfLine,
};

// - typing
// - transform/animation par glyph
// - textlog
// - line feed
// - page feed
// - line centering
/** UIMessageTextArea */
LN_CLASS()
class UIMessageTextArea
	: public UIElement
{
public:
	static Ref<UIMessageTextArea> create();

    /** setText */
    LN_METHOD(Property)
    void setText(const StringView& value);

    /** setTypingSpeed */
    LN_METHOD(Property)
	void setTypingSpeed(float value) { m_typingSpeed = value; }

    //void setViewportLineCount(int value) { m_viewportLineCount = value; }



LN_CONSTRUCT_ACCESS:
    UIMessageTextArea();

    /** init */
    LN_METHOD()
	bool init();

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
	Ref<detail::RTDocument> m_document;
	//Ref<detail::FlexText> m_flexText;
	float m_typingSpeed;
    String m_text;
    bool m_textDirty;
    //int m_viewportLineCount;
    //float m_viewportFitSize;
};

class UIMessageTextWindow
    : public UIWindow
{
public:
    static Ref<UIMessageTextWindow> create();

    void setText(const StringView& value);
    void setTypingSpeed(float value);


LN_CONSTRUCT_ACCESS:
    UIMessageTextWindow();
    void init();

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    virtual void onRender(UIRenderingContext* context) override;

private:
    Ref<detail::RTDocument> m_document;
};







namespace detail {

class RTDocument;

// 単一文字。最小単位。
struct RTGlyph
{
    uint32_t codePoint = 0;
    Vector2 localPos;   // 矩形の左上
    Size size;
    //Vector2 basePoint;  // グリフの中央
    //Vector2 baseOffset;
    float timeOffset = 0.0f;
    Matrix transform;
    Color color = Color::White;
};

struct RTLayoutContext
{
    RTDocument* document;
    UILayoutContext* layoutContext;
    float timeOffset;
};

class RTTextElement
    : public Object
{
public:

    /** フォントファミリ名を設定します。*/
    void setFontFamily(const Optional_deprecated<String>& value) { m_fontFamily = value; }

    /** フォントファミリ名を取得します。*/
    const Optional_deprecated<String>& getFontFamily() const { return m_fontFamily; }

    /** フォントサイズを設定します。*/
    void setFontSize(const Optional_deprecated<float>& value) { m_fontSize = value; }

    /** フォントサイズを取得します。*/
    const Optional_deprecated<float>& getFontSize() const { return m_fontSize; }

    /** フォントの太字有無を設定します。*/
    void setFontBold(const Optional_deprecated<UIFontWeight>& value) { m_fontWeight = value; }

    /** フォントの太字有無を取得します。*/
    const Optional_deprecated<UIFontWeight>& fontWeight() const { return m_fontWeight; }

    /** フォントのイタリック体有無を設定します。*/
    void setFontItalic(const Optional_deprecated<UIFontStyle>& value) { m_fontStyle = value; }

    /** フォントのイタリック体有無を取得します。*/
    const Optional_deprecated<UIFontStyle>& isFontItalic() const { return m_fontStyle; }

    ///** フォントのアンチエイリアス有無を設定します。*/
    //void setFontAntiAlias(bool value) { m_fontData.isAntiAlias = value; m_fontDataModified = true; }

    ///** フォントのアンチエイリアス有無を取得します。*/
    //bool isFontAntiAlias() const { return m_fontData.isAntiAlias; }

    virtual void updateFont(const Font* parentFinalFont, const detail::FontDesc& defaultFont, float dpiScale);
    const Ref<Font>& finalFont() const { return m_finalFont; }

    virtual void layout(RTLayoutContext* context) { }
    virtual void render(UIRenderingContext* context, RTDocument* document, const Point& globalOffset) {}

    const Size& extentSize() const { return m_extentSize; }
    const Size& actualSize() const { return m_actualSize; }

LN_CONSTRUCT_ACCESS:
    RTTextElement() {}
    virtual ~RTTextElement() {}
    void init() { Object::init(); }

protected:
    void setExtentSize(const Size& size) { m_extentSize = size; }
    void setActualSize(const Size& size) { m_actualSize = size; }

private:
    Optional_deprecated<String> m_fontFamily;
    Optional_deprecated<float> m_fontSize;
    Optional_deprecated<UIFontWeight> m_fontWeight;
    Optional_deprecated<UIFontStyle> m_fontStyle;
    Ref<Font> m_finalFont;	// TODO: これと、updateFontDescHierarchical での作成処理は Run にだけあればいいので、派生側に移動した方がメモリ消費少なくて済む
    Size m_extentSize;
    Size m_actualSize;
};

// インラインコンテンツのベース。
// Run や Link, IconImage, ルビ付きRun など。
// テキストの装飾範囲も兼ねる。
// WPF のフロードキュメントでは Inline(Span) のネストも可能であるが、ここではサポートしない。
class RTInline
    : public RTTextElement
{
public:
    RTInline();
    virtual ~RTInline() = default;
    void init() {}

private:
};


// 論理行
// https://docs.microsoft.com/ja-jp/dotnet/framework/wpf/advanced/flow-document-overview
class RTLineBlock
    : public RTTextElement
{
public:
    RTLineBlock() {}
    virtual ~RTLineBlock() = default;
    void init() {}

    void clearInlines();
    void addInline(RTInline* inl);
    const List<Ref<RTInline>>& inlines() const { return m_inlines; }

    virtual void updateFont(const Font* parentFinalFont, const detail::FontDesc& defaultFont, float dpiScale) override;
    virtual void layout(RTLayoutContext* context) override;
    virtual void render(UIRenderingContext* context, RTDocument* document, const Point& globalOffset) override;

private:
    List<Ref<RTInline>> m_inlines;
};


class RTDocument
    : public Object
{
public:
    RTDocument();
    void clear();
    void addBlock(RTLineBlock* block);

    void updateFont(const detail::FontDesc& defaultFont, float dpiScale);
    void updateFrame(float elapsedSeconds);
    Size measureLayout(UILayoutContext* context, const Size& constraint);
    Size arrangeLayout(UILayoutContext* context, const Size& areaSize);
    void render(UIRenderingContext* context);

    float typingSpeed() const { return m_typingSpeed; }
    float localTime() const { return m_localTime; }
    const Size& extentSize() const { return m_extentSize; }
    const Size& actualSize() const { return m_actualSize; }
    void setRenderOffset(const Point& value) { m_renderOffset = value; }
    const Point& renderOffset() const { return m_renderOffset; }
    //void setLineSpacing(float value) { m_lineSpacing = value; }
    //float lineSpacing() const { return m_lineSpacing; }

private:
    List<Ref<RTLineBlock>> m_blockList;
    float m_typingSpeed;
    float m_localTime;
    RTLayoutContext m_layoutContext;
    Size m_extentSize;  // 未タイプ部分も含めた全体サイズ
    Size m_actualSize;  // タイプされた部分のみのサイズ
    Point m_renderOffset;
    //float m_lineSpacing;
};

} // namespace detail
} // namespace ln



#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
//#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
//#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIFlexMessageTextArea.hpp>

#include "../Font/TextLayoutEngine.hpp"
#include "../Font/FontManager.hpp"

// TODO: Test
//#include <LuminoEngine/Graphics/Texture.hpp>
//#include <LuminoEngine/Rendering/Material.hpp>


namespace ln {
namespace detail {

//==============================================================================
// 表示用 (編集非対応) の Document
// このあたりのデータ構造は、FlexText を作成するために使う。
// 直接 FlexText を作ってしまってもいいのだが、テキストをレイアウトした情報は取っておいて
// UIElement サイズの計算に使ったりするので、FlexText だけだとちょっとつらい。
// かといって FlexText だけだと、Renderer に渡すデータ量が多くなってしまう。
// ※FlexText は描画必要な最低限のデータを持たせたい。キャッシュも行って使ったりする。
// ※FlexText の中身、つまり Glyph と Run は struct にして、コピーしやすくしたい。
//   一方こちら側は TextArea の変更に対応したり、ユーザーに公開してカスタマイズしやすくしたりと、Object である方が都合がいい。

class RTDocument;
class RTInline;



class RTRun
    : public RTInline
    , public MeasureTextLayoutEngine
{
public:
    RTRun();
    void init() { RTInline::init(); }

    void setText(const String& text);

    virtual void layout(RTLayoutContext* context, const Point& globalOffset) override;
    virtual void render(UIRenderingContext* context, RTDocument* document) override;

protected:
    virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

    // TextElement interface
    //virtual void onFontDataChanged(const ln::detail::FontData& newData) override;
    //virtual InternalTextElementType getInternalTextElementType() const override;

//LN_INTERNAL_ACCESS:
//	const UTF32* getText() const { return m_text.c_str(); }
//	int getLength() const { return m_text.getLength(); }

private:
    //ln::detail::SimpleStringBuilder<UTF32>	m_text;
    //Ref<GlyphRun>				m_glyphRun;
    String m_text;
    //Ref<detail::FontCore> m_rawFont;
    //List<RTGlyph> m_glyphs;
    //std::vector<detail::FlexGlyph> m_glyphs;
    //Ref<detail::FlexGlyphRun> m_flexGlyphRun;
    //RTDocument* m_layoutingDocument;
    Size m_layoutingSize;
    RTLayoutContext* m_layoutingContext;

    bool m_textDirty;

    List<RTGlyph> m_glyphs;
};

//class RTPhysicalLine
//    : public Object
//{
//public:
//    RTPhysicalLine(RTLineBlock* block);
//
//private:
//    RTLineBlock* m_block;
//    List<RTGlyph> m_glyphs;
//};

//==============================================================================
// RTTextElement

void RTTextElement::updateFont(const Font* parentFinalFont, const detail::FontDesc& defaultFont, float dpiScale)
{
	if (!m_finalFont) {
		m_finalFont = makeObject<Font>();
	}

	if (parentFinalFont) {
		m_finalFont->setFamily((m_fontFamily) ? m_fontFamily.value() : parentFinalFont->family());
		m_finalFont->setSize((m_fontSize) ? m_fontSize.value() : parentFinalFont->size());
		m_finalFont->setBold((m_fontWeight) ? (m_fontWeight.value() == UIFontWeight::Bold) : parentFinalFont->isBold());
		m_finalFont->setItalic((m_fontStyle) ? (m_fontStyle.value() == UIFontStyle::Italic) : parentFinalFont->isItalic());
	}
	else {
		m_finalFont->setFamily((m_fontFamily) ? m_fontFamily.value() : defaultFont.Family);
		m_finalFont->setSize((m_fontSize) ? m_fontSize.value() : defaultFont.Size);
		m_finalFont->setBold((m_fontWeight) ? (m_fontWeight.value() == UIFontWeight::Bold) : defaultFont.isBold);
		m_finalFont->setItalic((m_fontStyle) ? (m_fontStyle.value() == UIFontStyle::Italic) : defaultFont.isItalic);
    }
}

//==============================================================================
// RTInline

RTInline::RTInline()
{
}

//==============================================================================
// RTRun

RTRun::RTRun()
    : m_textDirty(true)
{
}

void RTRun::setText(const String& text)
{
    m_text = text;
    m_textDirty = true;
}

void RTRun::layout(RTLayoutContext* context, const Point& globalOffset)
{
    RTInline::layout(context, globalOffset);

    if (m_textDirty) {
        m_glyphs.clear();
        m_layoutingSize = Size::Zero;
        m_layoutingContext = context;

        detail::FontCore* fontCore = detail::FontHelper::resolveFontCore(finalFont(), context->layoutContext->dpiScale());
        TextLayoutEngine::layout(fontCore, m_text.c_str(), m_text.length(), Rect(0, 0, FLT_MAX, FLT_MAX), 0, TextAlignment::Left);

        m_textDirty = false;
    }
}

void RTRun::render(UIRenderingContext* context, RTDocument* document)
{
    RTInline::render(context, document);
    for (auto& glyph : m_glyphs) {
        if (glyph.timeOffset <= document->localTime()) {
            auto transform = Matrix::makeTranslation(Vector3(glyph.localPos, 0.0f));
            context->drawChar(glyph.codePoint, glyph.color, finalFont(), transform);
        }
    }
}

void RTRun::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
    RTGlyph glyph;
    glyph.codePoint = ch;
    glyph.localPos = pos;
    glyph.timeOffset = m_layoutingContext->timeOffset;
    glyph.transform = Matrix::Identity;
    glyph.color = Color::White;
    m_glyphs.add(glyph);

    m_layoutingSize.width += size.width;
    m_layoutingSize.height = std::max(m_layoutingSize.height, size.height);

    m_layoutingContext->timeOffset += m_layoutingContext->document->typingSpeed();
}

//==============================================================================
// RTLineBlock

void RTLineBlock::clearInlines()
{
	m_inlines.clear();
}

void RTLineBlock::addInline(RTInline* inl)
{
	if (LN_REQUIRE(inl)) return;
	m_inlines.add(inl);
}

void RTLineBlock::updateFont(const Font* parentFinalFont, const detail::FontDesc& defaultFont, float dpiScale)
{
    RTTextElement::updateFont(parentFinalFont, defaultFont, dpiScale);
	for (auto& inl : m_inlines) {
		inl->updateFont(parentFinalFont, defaultFont, dpiScale);
	}
}

void RTLineBlock::layout(RTLayoutContext* context, const Point& globalOffset)
{
    Point offset = globalOffset;
    Size size;
    for (auto& inl : m_inlines) {
        offset.x = size.width;
        inl->layout(context, offset);
        size.width += inl->desiredSize().width;
        size.height = std::max(size.height, inl->desiredSize().height);
    }
    setDesiredSize(size);
}

void RTLineBlock::render(UIRenderingContext* context, RTDocument* document)
{
    for (auto& inl : m_inlines) {
        inl->render(context, document);
    }
}

//==============================================================================
//

RTDocument::RTDocument()
	: m_typingSpeed(0.5f)
    , m_localTime(0.0f)
{
}

void RTDocument::clear()
{
	m_blockList.clear();
	m_localTime = 0.0f;
}

void RTDocument::addBlock(RTLineBlock* block)
{
	if (LN_REQUIRE(block)) return;
	m_blockList.add(block);
}

void RTDocument::updateFont(const detail::FontDesc& defaultFont, float dpiScale)
{
	for (auto& block : m_blockList) {
		block->updateFont(nullptr, defaultFont, dpiScale);
	}
}

void RTDocument::updateFrame(float elapsedSeconds)
{
	m_localTime += elapsedSeconds;
}

Size RTDocument::measureLayout(UILayoutContext* context, const Size& constraint)
{
    m_layoutContext.document = this;
    m_layoutContext.layoutContext = context;
    m_layoutContext.timeOffset = 0.0f;

    Size size;
    for (auto& block : m_blockList) {
        block->layout(&m_layoutContext, Point::Zero);
        size.width += block->desiredSize().width;
        size.height = std::max(size.height, block->desiredSize().height);
    }
	return size;
}

Size RTDocument::arrangeLayout(UILayoutContext* context, const Size& areaSize)
{
	return areaSize;
}

void RTDocument::render(UIRenderingContext* context)
{
    for (auto& block : m_blockList) {
        block->render(context, this);
    }
}

//==============================================================================
// RTPhysicalLine

//RTPhysicalLine::RTPhysicalLine(RTLineBlock* block)
//{
//}

//==============================================================================
// RTDocumentBuilder

class RTDocumentBuilder
{
public:
	RTDocumentBuilder(RTDocument* doc);
	void parse(Font* font, float dpiScale, const ln::String& text, const Size& areaSize);

private:

	RTDocument* m_document;
	//RTRun* m_currentRun;
	//float m_timeOffset;
};


RTDocumentBuilder::RTDocumentBuilder(RTDocument* doc)
	: m_document(doc)
	//, m_currentRun(nullptr)
{
}

void RTDocumentBuilder::parse(Font* font, float dpiScale, const ln::String& text, const Size& areaSize)
{
	m_document->clear();
	//m_timeOffset = 0.0f;

	auto p = makeObject<RTLineBlock>();
	m_document->addBlock(p);
	auto r = makeObject<RTRun>();
    r->setText(text);
	p->addInline(r);
	//m_currentRun = r;
}

} // namespace detail

//==============================================================================
// UIMessageTextArea

Ref<UIMessageTextArea> UIMessageTextArea::create()
{
	return makeObject<UIMessageTextArea>();
}

UIMessageTextArea::UIMessageTextArea()
	: m_typingSpeed(0.05f)
    , m_textDirty(false)
{
}

void UIMessageTextArea::init()
{
	UIElement::init();
	m_document = makeObject<detail::RTDocument>();


	//auto p = makeObject<RTParagraph>();
	//m_document->addBlock(p);

	//auto r = makeObject<RTRun>();
	//r->setText(u"Run Test");
	//p->addInline(r);
}

void UIMessageTextArea::setText(const StringRef& value)
{
    m_text = value;
    m_textDirty = true;
}

Size UIMessageTextArea::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    if (m_textDirty) {
        detail::RTDocumentBuilder builder(m_document);
        builder.parse(finalStyle()->font, layoutContext->dpiScale(), m_text, constraint);
        m_textDirty = false;
    }

    m_document->updateFont(detail::FontHelper::getFontDesc(finalStyle()->font), layoutContext->dpiScale());

	Size baseArea = Size::max(constraint, UIElement::measureOverride(layoutContext, constraint));


	//return m_document->measureLayout(constraint);
	return Size::min(m_document->measureLayout(layoutContext, constraint), baseArea);
	//return UIElement::measureOverride(constraint);
}

Size UIMessageTextArea::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
	return Size::min(m_document->arrangeLayout(layoutContext, finalSize), UIElement::arrangeOverride(layoutContext, finalSize));
	//return UIElement::arrangeOverride(finalSize);
}

void UIMessageTextArea::onUpdateFrame(float elapsedSeconds)
{
	m_document->updateFrame(elapsedSeconds * (1.0f / m_typingSpeed));
}

void UIMessageTextArea::onRender(UIRenderingContext* context)
{
	m_document->render(context);
}

} // namespace ln



#include "Internal.hpp"
#include <algorithm>
#include <LuminoGraphics/Font/Font.hpp>
//#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
//#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include "UIEditableTextArea.hpp"
#include <LuminoEngine/UI/UIMessageTextArea.hpp>

#include <LuminoGraphics/Font/detail/TextLayoutEngine.hpp>
#include <LuminoGraphics/Font/detail/FontManager.hpp>
#include "UIStyleInstance.hpp"

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

    virtual void layout(RTLayoutContext* context) override;
    virtual void render(UIRenderingContext* context, RTDocument* document, const Point& globalOffset) override;

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
    //Size m_layoutingSize;
    RTLayoutContext* m_layoutingContext;


    bool m_textDirty;

    List<RTGlyph> m_glyphs;
    //Size m_actualSize;
    float m_crossSize;
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
		m_finalFont = makeObject_deprecated<Font>();
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
    , m_crossSize(0.0f)
{
}

void RTRun::setText(const String& text)
{
    m_text = text;
    m_textDirty = true;
}

void RTRun::layout(RTLayoutContext* context)
{
    RTInline::layout(context);

    if (m_textDirty) {
        m_glyphs.clear();
        //m_layoutingSize = Size::Zero;
        m_layoutingContext = context;

        detail::FontCore* fontCore = detail::FontHelper::resolveFontCore(finalFont(), context->layoutContext->dpiScale());
        TextLayoutEngine::layout(fontCore, m_text.c_str(), m_text.length(), Rect(0, 0, std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), 0, TextAlignment::Forward);
        //setExtentSize(m_layoutingSize);

        detail::FontGlobalMetrics metrics;
        fontCore->getGlobalMetrics(&metrics);
        m_crossSize = metrics.lineSpace;

        m_textDirty = false;
    }

    //auto extentSize = Size::Zero;
    //auto actualSize = Size::Zero;
    float mainSize = 0.0f;
    if (m_glyphs.isEmpty()) {
        //actualSize.height = context->document->lineSpacing();
    }
    else {
        for (auto& glyph : m_glyphs) {

            if (glyph.timeOffset <= context->document->localTime()) {
                mainSize = std::max(mainSize, glyph.localPos.x + glyph.size.width);
                //actualSize.width = std::max(actualSize.width, glyph.localPos.x + glyph.size.width);
                //actualSize.height = extentSize().height;//context->document->lineSpacing();
            }
        }
    }

    setActualSize(Size(mainSize, m_crossSize));
}

void RTRun::render(UIRenderingContext* context, RTDocument* document, const Point& globalOffset)
{
    RTInline::render(context, document, globalOffset);
    float localTime = document->localTime();

    for (auto& glyph : m_glyphs) {
        if (glyph.timeOffset <= document->localTime()) {
            //float a = Math::clamp(localTime - glyph.timeOffset, 0.0f, 0.5f) * 2.0f;
            //float a = Math::clamp(localTime - glyph.timeOffset, 0.0f, 1.0f);
            //float a = Math::clamp(localTime - glyph.timeOffset, 0.0f, 2.0f) * 0.5f;
            float a = Math::clamp(localTime - glyph.timeOffset, 0.0f, 5.0f) * 0.2f; // 5s

            auto transform = Matrix::makeTranslation(Vector3(globalOffset.x + glyph.localPos.x, globalOffset.y + glyph.localPos.y, 0));
            context->drawChar(glyph.codePoint, glyph.color.withAlpha(a), finalFont(), transform);
        }
    }
}

void RTRun::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
    RTGlyph glyph;
    glyph.codePoint = ch;
    glyph.localPos = pos;
    glyph.size = size;
    glyph.timeOffset = m_layoutingContext->document->localTime() + m_layoutingContext->timeOffset;
    glyph.transform = Matrix::Identity;
    glyph.color = Color::White;
    m_glyphs.add(glyph);

    Size es = extentSize();
    es.width = std::max(es.width, glyph.localPos.x + glyph.size.width);
    es.height = std::max(es.height, glyph.localPos.y + glyph.size.height);
    setExtentSize(es);
    //m_layoutingSize.width = std::max(m_layoutingSize.width, pos.x + size.width);
    //m_layoutingSize.height = std::max(m_layoutingSize.height, pos.y + size.height);

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

void RTLineBlock::layout(RTLayoutContext* context)
{
    auto extentSize = Size::Zero;
    auto actualSize = Size::Zero;
    for (auto& inl : m_inlines) {
        inl->layout(context);
        extentSize.width += inl->extentSize().width;
        extentSize.height = std::max(extentSize.height, inl->extentSize().height);
        actualSize.width += inl->actualSize().width;
        actualSize.height = std::max(actualSize.height, inl->actualSize().height);
    }
    setExtentSize(extentSize);
    setActualSize(actualSize);
}

void RTLineBlock::render(UIRenderingContext* context, RTDocument* document, const Point& globalOffset)
{
    auto offset = globalOffset;
    for (auto& inl : m_inlines) {
        inl->render(context, document, offset);
        offset.x = inl->extentSize().width;
        offset.y = inl->extentSize().height;
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

    auto extentSize = Size::Zero;
    auto actualSize = Size::Zero;
    for (auto& block : m_blockList) {
        block->layout(&m_layoutContext);
        extentSize.width = std::max(extentSize.width, block->extentSize().width);
        extentSize.height += block->extentSize().height;
        actualSize.width = std::max(actualSize.width, block->actualSize().width);
        actualSize.height += block->actualSize().height;
    }
    m_extentSize = extentSize;
    m_actualSize = actualSize;

	return m_extentSize;
}

Size RTDocument::arrangeLayout(UILayoutContext* context, const Size& areaSize)
{
	return areaSize;
}

void RTDocument::render(UIRenderingContext* context)
{
    auto offset = m_renderOffset;
    for (auto& block : m_blockList) {
        block->render(context, this, offset);
        offset.y += block->actualSize().height;
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
	void parse(Font* font, const ln::String& text);

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

void RTDocumentBuilder::parse(Font* font, const ln::String& text)
{
	m_document->clear();
	//m_timeOffset = 0.0f;

    List<UITextRange> lines;
    UITextRange::splitLineRanges(text, &lines);
    for (auto& line : lines) {
        auto p = makeObject_deprecated<RTLineBlock>();
        m_document->addBlock(p);
        auto r = makeObject_deprecated<RTRun>();
        r->setText(text.substr(line.beginIndex, line.length()));
        p->addInline(r);
    }

	//m_currentRun = r;
}

} // namespace detail

//==============================================================================
// UIMessageTextArea

Ref<UIMessageTextArea> UIMessageTextArea::create()
{
	return makeObject_deprecated<UIMessageTextArea>();
}

UIMessageTextArea::UIMessageTextArea()
	: m_typingSpeed(0.05f)
    , m_textDirty(false)
    //, m_viewportLineCount(0)
{
}

bool UIMessageTextArea::init()
{
    if (!UIElement::init()) return false;

	m_document = makeObject_deprecated<detail::RTDocument>();


	//auto p = makeObject_deprecated<RTParagraph>();
	//m_document->addBlock(p);

	//auto r = makeObject_deprecated<RTRun>();
	//r->setText(u"Run Test");
	//p->addInline(r);

    return true;
}

void UIMessageTextArea::setText(const StringView& value)
{
    m_text = value;
    m_textDirty = true;
}

void UIMessageTextArea::onUpdateFrame(float elapsedSeconds)
{
    m_document->updateFrame(elapsedSeconds * (1.0f / m_typingSpeed));


    //float diffY = m_document->extentSize().height - m_document->actualSize().height;
    //std::cout << actualSize().height << std::endl;
    //std::cout << diffY << std::endl;
    //m_document->setRenderOffset(Point(0, actualSize().height - (actualSize().height - diffY)));

}

Size UIMessageTextArea::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    if (m_textDirty) {
        detail::RTDocumentBuilder builder(m_document);
        builder.parse(finalStyle()->font, m_text);
        m_textDirty = false;
    }

    m_document->updateFont(detail::FontHelper::getFontDesc(finalStyle()->font), layoutContext->dpiScale());

	Size baseArea = Size::max(constraint, UIElement::measureOverride(layoutContext, constraint));


    //if (m_viewportLineCount > 0) {
    //    auto core = detail::FontHelper::resolveFontCore(finalStyle()->font, layoutContext->dpiScale());
    //    detail::FontGlobalMetrics metrics;
    //    core->getGlobalMetrics(&metrics);

    //    m_document->setLineSpacing(metrics.lineSpace);
    //    m_viewportFitSize = metrics.lineSpace * m_viewportLineCount;
    //    // TODO: ルビの分の高さ（使わなくても常にその分のサイズをとっておき、すべての行のサイズは同じになるような仕様としたい）
   
    //    baseArea.height = m_viewportFitSize;
    //}


	return Size::min(m_document->measureLayout(layoutContext, constraint), baseArea);
}

Size UIMessageTextArea::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();

	auto size = Size::min(m_document->arrangeLayout(layoutContext, finalSize), UIElement::arrangeOverride(layoutContext, finalArea));
    //if (m_viewportLineCount > 0) {
    //    size.height = m_viewportFitSize;
    //}


    return size;
}

void UIMessageTextArea::onRender(UIRenderingContext* context)
{
    //m_document->setRenderOffset(Point(0, actualSize().height - m_document->actualSize().height));

	m_document->render(context);
}

//==============================================================================
// UIMessageTextWindow

Ref<UIMessageTextWindow> UIMessageTextWindow::create()
{
    return makeObject_deprecated<UIMessageTextWindow>();
}

UIMessageTextWindow::UIMessageTextWindow()
{
}

void UIMessageTextWindow::init()
{
    UIElement::init();
    m_document = makeObject_deprecated<detail::RTDocument>();
}

void UIMessageTextWindow::setText(const StringView& value)
{
    detail::RTDocumentBuilder builder(m_document);
    builder.parse(finalStyle()->font, value);
}

//void UIMessageTextWindow::setTypingSpeed(float value)
//{
//}

void UIMessageTextWindow::onUpdateFrame(float elapsedSeconds)
{
    m_document->updateFrame(elapsedSeconds);
}

Size UIMessageTextWindow::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    m_document->updateFont(detail::FontHelper::getFontDesc(finalStyle()->font), layoutContext->dpiScale());

    Size baseArea = Size::max(constraint, UIElement::measureOverride(layoutContext, constraint));


    return Size::min(m_document->measureLayout(layoutContext, constraint), baseArea);
}

Size UIMessageTextWindow::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{

    auto size = Size::min(m_document->arrangeLayout(layoutContext, finalArea.getSize()), UIElement::arrangeOverride(layoutContext, finalArea));


    return size;
}

void UIMessageTextWindow::onRender(UIRenderingContext* context)
{
    m_document->render(context);
}

} // namespace ln


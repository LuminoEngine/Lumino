
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>

#include "../Font/TextLayoutEngine.hpp"
#include "../Font/FontManager.hpp"

// TODO: Test
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>


namespace ln {

//==============================================================================
// UITextBlock

Ref<UITextBlock> UITextBlock::create()
{
    return makeObject<UITextBlock>();
}

UITextBlock::UITextBlock()
{
}

void UITextBlock::init()
{
    UIElement::init();
    setBlendMode(BlendMode::Alpha);
}

Size UITextBlock::measureOverride(const Size& constraint)
{
	float scale = 1.0f;
	if (UIFrameWindow* w = static_cast<UIFrameWindow*>(getFrameWindow())) {
		scale = w->platformWindow()->dpiFactor();
	}

	return finalStyle()->font->measureRenderSize(m_text, scale);
}

void UITextBlock::onRender(UIRenderingContext* context)
{
    //{
    //    auto tex = makeObject<Texture2D>(u"D:/Proj/LN/HC1/Assets/Windowskin/window.png");
    //    auto mat = Material::create(tex);
    //    context->drawBoxBackground(Rect(10, 20, 100, 200), Thickness(16), CornerRadius(), BrushImageDrawMode::BorderFrame, mat, Rect(64, 0, 64, 64));
    //}


    context->drawText(m_text, finalStyle()->textColor, finalStyle()->font);
}

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

// 単一文字。最小単位。
//struct RTGlyph
//{
//	Vector2 pos;
//	uint32_t codePoint;
//};
//class RTGlyph
//	: public Object
//{
//public:
//	RTGlyph() {}
//	void init() { Object::init(); }
//
//private:
//};

class RTTextElement
	: public Object
{
public:

	/** フォントファミリ名を設定します。*/
	void setFontFamily(const Optional<String>& value) { m_fontFamily = value; }

	/** フォントファミリ名を取得します。*/
	const Optional<String>& getFontFamily() const { return m_fontFamily; }

	/** フォントサイズを設定します。*/
	void setFontSize(const Optional<float>& value) { m_fontSize = value; }

	/** フォントサイズを取得します。*/
	const Optional<float>& getFontSize() const { return m_fontSize; }

	/** フォントの太字有無を設定します。*/
	void setFontBold(const Optional<UIFontWeight>& value) { m_fontWeight = value; }

	/** フォントの太字有無を取得します。*/
	const Optional<UIFontWeight>& fontWeight() const { return m_fontWeight; }

	/** フォントのイタリック体有無を設定します。*/
	void setFontItalic(const Optional<UIFontStyle>& value) { m_fontStyle = value; }

	/** フォントのイタリック体有無を取得します。*/
	const Optional<UIFontStyle>& isFontItalic() const { return m_fontStyle; }

	///** フォントのアンチエイリアス有無を設定します。*/
	//void setFontAntiAlias(bool value) { m_fontData.isAntiAlias = value; m_fontDataModified = true; }

	///** フォントのアンチエイリアス有無を取得します。*/
	//bool isFontAntiAlias() const { return m_fontData.isAntiAlias; }

//	Brush* getForeground() const;
//
//protected:
//	virtual void onFontDataChanged(const ln::detail::FontData& newData);

	virtual void updateFontDescHierarchical(const RTTextElement* parent, const detail::FontDesc& defaultFont, float dpiScale);
	const detail::FontDesc& finalFontDesc() const { return m_finalFontDesc; }


	virtual Size measureLayout(const Size& constraint) { return Size::Zero; }
	virtual Size arrangeLayout(const Size& areaSize) { return Size::Zero; }

LN_CONSTRUCT_ACCESS:
	RTTextElement() {}
	virtual ~RTTextElement() {}
	void init() { Object::init(); }

LN_INTERNAL_ACCESS:
	//ln::detail::DocumentsManager* getManager() const { return m_manager; }
	//virtual InternalTextElementType getInternalTextElementType() const;
	//void setParent(TextElement* parent) { m_parent = parent; }
	//TextElement* GetParent() const { return m_parent; }
	//int getThisRevision() const { return m_thisRevision; }
	//int getChildrenRevision() const { return m_childrenRevision; }
	//bool isDeleted() const { return m_deleted; }

private:
	Optional<String> m_fontFamily;
	Optional<float> m_fontSize;
	Optional<UIFontWeight> m_fontWeight;
	Optional<UIFontStyle> m_fontStyle;
	detail::FontDesc m_finalFontDesc;

	//ln::detail::DocumentsManager*		m_manager;
	//TextElement*						m_parent;
	//ln::detail::FontData				m_fontData;
	//Ref<Brush>			m_foreground;
	//bool					m_fontDataModified;


	//int								m_thisRevision;
	//int								m_childrenRevision;

	//bool					m_deleted;
};



class RTBlock
	: public RTTextElement
{
public:
	RTBlock() {}
	virtual ~RTBlock() = default;
	void init() {}

	void addInline(RTInline* inl);
	void clearInlines();

	void updateVisualData(RTDocument* document, const Size& areaSize, const Vector2& offset);

	virtual void updateFontDescHierarchical(const RTTextElement* parent, const detail::FontDesc& defaultFont, float dpiScale) override;

protected:

LN_INTERNAL_ACCESS:
	//void insertInlines(int index, const List<Ref<RTInline>>& inlines);
	const List<Ref<RTInline>>& getInlines() const { return m_inlines; }

private:
	List<Ref<RTInline>> m_inlines;
};

/** コンテンツをグループ化して段落にするために使用される。 */
class RTParagraph
	: public RTBlock
{
public:
	RTParagraph() {}
	void init() { RTBlock::init(); }


private:
};


class RTInline
	: public RTTextElement
{
public:
	RTInline() {}
	virtual ~RTInline() = default;
	void init() {}

	virtual void updateVisualData(RTDocument* document, const Size& areaSize, const Vector2& offset) {}

private:
};

class RTRun
	: public RTInline
	, public detail::MeasureTextLayoutEngine
{
public:
	RTRun();
	void init() { RTInline::init(); }

	void setText(const StringRef& text) { m_text = text; }

	virtual void updateFontDescHierarchical(const RTTextElement* parent, const detail::FontDesc& defaultFont, float dpiScale) override
	{
		RTInline::updateFontDescHierarchical(parent, defaultFont, dpiScale);
		m_rawFont = detail::EngineDomain::fontManager()->lookupFontCore(finalFontDesc(), dpiScale);
	}

	virtual void updateVisualData(RTDocument* document, const Size& areaSize, const Vector2& offset) override;

	void addGlyph(uint32_t codePoint, const Vector3& pos, float timeOffset);

	virtual Size measureLayout(const Size& constraint) override;
	virtual Size arrangeLayout(const Size& areaSize) override;

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
	Ref<detail::FontCore> m_rawFont;
	//List<RTGlyph> m_glyphs;
	std::vector<detail::FlexGlyph> m_glyphs;
	//Ref<detail::FlexGlyphRun> m_flexGlyphRun;
	RTDocument* m_layoutingDocument;
	bool m_dirty;
};

// 装飾範囲
class RTSpan
	: public RTInline
{
public:
	RTSpan();
	virtual ~RTSpan();
	void initialize();

private:
	// Inline List
};


class RTLineBreak
	: public RTInline
{
public:
	RTLineBreak();
	virtual ~RTLineBreak();
	void initialize();

private:
	//virtual InternalTextElementType getInternalTextElementType() const;
};


class RTDocument
	: public Object
{
public:
	RTDocument();
	void clear();
	void addBlock(RTBlock* block);
	void updateFontDesc(const detail::FontDesc& defaultFont, float dpiScale);
	void updateFrame(float elapsedSeconds);
	Size measureLayout(const Size& constraint);
	Size arrangeLayout(const Size& areaSize);
	//void updateGlyphs();
	void render(UIRenderingContext* context);

	//void addGlyph(const RTGlyph& value) { m_glyphs.add(value); }
	const Ref<detail::FlexText>& flexText() const { return m_flexText; }

private:
	List<Ref<RTBlock>> m_blockList;
	//List<RTGlyph> m_glyphs;
	Ref<detail::FlexText> m_flexText;
	float m_localTime;
};



//==============================================================================
// RTTextElement

void RTTextElement::updateFontDescHierarchical(const RTTextElement* parent, const detail::FontDesc& defaultFont, float dpiScale)
{
	if (parent) {
		m_finalFontDesc.Family = (m_fontFamily) ? m_fontFamily.value() : parent->m_finalFontDesc.Family;
		m_finalFontDesc.Size = (m_fontSize) ? m_fontSize.value() : parent->m_finalFontDesc.Size;
		m_finalFontDesc.isBold = (m_fontWeight) ? (m_fontWeight.value() == UIFontWeight::Bold) : parent->m_finalFontDesc.isBold;
		m_finalFontDesc.isItalic = (m_fontStyle) ? (m_fontStyle.value() == UIFontStyle::Italic) : parent->m_finalFontDesc.isItalic;
	}
	else {
		m_finalFontDesc.Family = (m_fontFamily) ? m_fontFamily.value() : defaultFont.Family;
		m_finalFontDesc.Size = (m_fontSize) ? m_fontSize.value() : defaultFont.Size;
		m_finalFontDesc.isBold = (m_fontWeight) ? (m_fontWeight.value() == UIFontWeight::Bold) : defaultFont.isBold;
		m_finalFontDesc.isItalic = (m_fontStyle) ? (m_fontStyle.value() == UIFontStyle::Italic) : defaultFont.isItalic;
	}
}

//==============================================================================
// RTBlock

void RTBlock::addInline(RTInline* inl)
{
	if (LN_REQUIRE(inl)) return;
	m_inlines.add(inl);
}

void RTBlock::clearInlines()
{
	m_inlines.clear();
}

void RTBlock::updateVisualData(RTDocument* document, const Size& areaSize, const Vector2& offset)
{
	for (auto& inl : m_inlines) {
		inl->updateVisualData(document, areaSize, offset);
	}
}

void RTBlock::updateFontDescHierarchical(const RTTextElement* parent, const detail::FontDesc& defaultFont, float dpiScale)
{
	RTTextElement::updateFontDescHierarchical(parent, defaultFont, dpiScale);
	for (auto& inl : m_inlines) {
		inl->updateFontDescHierarchical(this, defaultFont, dpiScale);
	}
}

//==============================================================================
// RTRun

RTRun::RTRun()
	: m_dirty(true)
	//: m_flexGlyphRun(makeRef<detail::FlexGlyphRun>())
{
}

void RTRun::updateVisualData(RTDocument* document, const Size& areaSize, const Vector2& offset)
{
#if 0
	m_glyphs.clear();
	m_layoutingDocument = document;

	layout(m_rawFont, m_text.c_str(), m_text.length(), Rect(0, 0, areaSize), 0, TextAlignment::Left);
	m_layoutingDocument->flexText()->addGlyphRun(m_glyphs.data(), m_glyphs.size(), m_rawFont, Color::White);

	m_layoutingDocument = nullptr;
	//void layout(FontCore* font, const Char* text, size_t length, const Rect& targetArea, float strokeSize, TextAlignment alignment);
	// layout;

#endif

	//if (m_dirty) {
		document->flexText()->addGlyphRun(m_glyphs.data(), m_glyphs.size(), m_rawFont, Color::White);
	//	m_dirty = false;
	//}
}

void RTRun::addGlyph(uint32_t codePoint, const Vector3& pos, float timeOffset)
{
	detail::FlexGlyph g;
	g.codePoint = codePoint;
	g.pos = pos;
	g.timeOffset = timeOffset;
	m_glyphs.push_back(g);
	//m_layoutingDocument->flexText()->addGlyphRun(g, );
	m_dirty = true;
}

void RTRun::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
	//RTGlyph g;
	//g.pos = pos;
	//g.codePoint = ch;
	//m_layoutingDocument->addGlyph(g);
	detail::FlexGlyph g;
	g.codePoint = ch;
	g.pos = Vector3(pos, 0);
	g.timeOffset = 0;
	m_glyphs.push_back(g);
	//m_layoutingDocument->flexText()->addGlyphRun(g,)
}

Size RTRun::measureLayout(const Size& constraint)
{
	return constraint;
}

Size RTRun::arrangeLayout(const Size& areaSize)
{
	return areaSize;
}

//==============================================================================
//

RTDocument::RTDocument()
	: m_flexText(makeRef<detail::FlexText>())
	, m_localTime(0.0f)
{
}

void RTDocument::clear()
{
	m_blockList.clear();
	m_flexText->clear();
	m_localTime = 0.0f;
}

void RTDocument::addBlock(RTBlock* block)
{
	if (LN_REQUIRE(block)) return;
	m_blockList.add(block);
}

void RTDocument::updateFontDesc(const detail::FontDesc& defaultFont, float dpiScale)
{
	for (auto& block : m_blockList) {
		block->updateFontDescHierarchical(nullptr, defaultFont, dpiScale);
	}
}

void RTDocument::updateFrame(float elapsedSeconds)
{
	m_localTime += elapsedSeconds;
	m_flexText->setTime(m_localTime);
}

// TODO: 折り返しする場合は constraint の幅で折り返し、下方向に改行した分で全体サイズを返す。
Size RTDocument::measureLayout(const Size& constraint)
{
	return constraint;
}

Size RTDocument::arrangeLayout(const Size& areaSize)
{
	//m_glyphs.clear();
	m_flexText->clear();
	for (auto& block : m_blockList) {
		block->updateVisualData(this, areaSize, Vector2::Zero);
	}

	return areaSize;
}

//void RTDocument::updateGlyphs()
//{
//	m_glyphs.clear();
//}

void RTDocument::render(UIRenderingContext* context)
{
	//context->setBlendMode(BlendMode::Normal);
	for (auto& run : m_flexText->glyphRuns()) {
		context->drawFlexGlyphRun(&run);
	}
	//for (auto& glyph : m_glyphs) {
	//	Char ch = glyph.codePoint;
	//	context->setBaseTransfrom(Matrix::makeTranslation(Vector3(glyph.pos, 0)));
	//	context->drawText(StringRef(&ch, 1), Color::White);
	//}
}


//==============================================================================
// RTDocumentBuilder

class RTDocumentBuilder
	: public detail::TextLayoutEngine
{
public:
	RTDocumentBuilder(RTDocument* doc);
	void parse(Font* font, float dpiScale, const ln::String& text, const Size& areaSize);

private:
	virtual void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

	RTDocument* m_document;
	RTRun* m_currentRun;
	float m_timeOffset;
};


RTDocumentBuilder::RTDocumentBuilder(RTDocument* doc)
	: m_document(doc)
	, m_currentRun(nullptr)
{
}

void RTDocumentBuilder::parse(Font* font, float dpiScale, const ln::String& text, const Size& areaSize)
{
	m_document->clear();
	m_timeOffset = 0.0f;

	auto p = makeObject<RTParagraph>();
	m_document->addBlock(p);
	auto r = makeObject<RTRun>();
	p->addInline(r);
	m_currentRun = r;

	detail::FontCore* fontCore = detail::FontHelper::resolveFontCore(font, dpiScale);
	layout(fontCore, text.c_str(), text.length(), Rect(0, 0, areaSize), 0, TextAlignment::Left);
}

void RTDocumentBuilder::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
	m_currentRun->addGlyph(ch, Vector3(pos, 0), m_timeOffset);
	m_timeOffset += 1.0f;
}


//==============================================================================
// UITypographyArea

Ref<UITypographyArea> UITypographyArea::create()
{
	return makeObject<UITypographyArea>();
}

UITypographyArea::UITypographyArea()
	: m_typingSpeed(0.05f)
{
}

void UITypographyArea::init()
{
	UIElement::init();
	m_document = makeObject<RTDocument>();


	//auto p = makeObject<RTParagraph>();
	//m_document->addBlock(p);

	//auto r = makeObject<RTRun>();
	//r->setText(u"Run Test");
	//p->addInline(r);
}

Size UITypographyArea::measureOverride(const Size& constraint)
{
	float dpiScale = 1.0f;
	if (UIFrameWindow* w = static_cast<UIFrameWindow*>(getFrameWindow())) {
		dpiScale = w->platformWindow()->dpiFactor();
	}


	static bool init = false;
	if (!init) {
		RTDocumentBuilder builder(m_document);
		builder.parse(finalStyle()->font, dpiScale, u"Hello Text!", constraint);
		init = true;
	}

	// TODO: 構築後、最初の１回だけでよい
	m_document->updateFontDesc(detail::FontHelper::getFontDesc(finalStyle()->font), dpiScale);


	Size baseArea = Size::max(constraint, UIElement::measureOverride(constraint));


	//return m_document->measureLayout(constraint);
	return Size::min(m_document->measureLayout(constraint), baseArea);
	//return UIElement::measureOverride(constraint);
}

Size UITypographyArea::arrangeOverride(const Size& finalSize)
{
	return Size::min(m_document->arrangeLayout(finalSize), UIElement::arrangeOverride(finalSize));
	//return UIElement::arrangeOverride(finalSize);
}

void UITypographyArea::onUpdateFrame(float elapsedSeconds)
{
	m_document->updateFrame(elapsedSeconds * (1.0f / m_typingSpeed));
}

void UITypographyArea::onRender(UIRenderingContext* context)
{
	m_document->render(context);
}

} // namespace ln


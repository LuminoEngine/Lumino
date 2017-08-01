
#include "Internal.h"
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "DocumentsManager.h"
#include "DocumentElements.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// Document
//==============================================================================

//------------------------------------------------------------------------------
Document::Document()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
Document::~Document()
{
}

//------------------------------------------------------------------------------
void Document::initialize()
{
	m_manager = detail::DocumentsManager::getInstance();
}

//------------------------------------------------------------------------------
void Document::setText(const StringRef& text)
{
	m_blockList.clear();


}

//------------------------------------------------------------------------------
void Document::replace(int offset, int length, const StringRef& text)
{
	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->getTCharToUTF32Converter()->convert(text.getBegin(), sizeof(TCHAR) * text.getLength());
	int len = utf32Buf.getSize() / sizeof(UTF32);
	replace(offset, length, (const UTF32*)utf32Buf.getConstData(), len);
}

//------------------------------------------------------------------------------
void Document::replace(int offset, int length, const UTF32* text, int len)
{
	LN_ASSERT(offset == 0 && length == 0);	// TODO: まだ

	// text を run と LineBrake のリストにする
	List<Ref<Inline>> inlines;
	{
		const UTF32* pos = text;
		const UTF32* end = pos + len;
		int nlIndex = 0;
		int nlCount = 0;
		while (StringTraits::indexOfNewLineSequence(pos, end, &nlIndex, &nlCount))
		{
			inlines.add(newObject<run>(pos, nlIndex).get());
			inlines.add(newObject<LineBreak>().get());
			pos += (nlIndex + nlCount);	// 改行文字の次の文字を指す
		}
		if (pos != end)
		{
			inlines.add(newObject<run>(pos, nlIndex).get());
		}
	}

	// TODO: Insert 先を割る
	int localInsertPoint = 0;
	LN_ASSERT(m_blockList.isEmpty());	// TODO
	Ref<Block> parentBlock = newObject<Paragraph>();
	m_blockList.add(parentBlock);

	parentBlock->insertInlines(localInsertPoint, inlines);


	// TODO: マージする

	
	//// Insert 先検索
	//if (0)
	//{

	//}
	//else
	//{

	//	m_blockList.Add();
	//}





	//int beginLineNumber, beginPosFromLineHead;
	//int endLineNumber, endPosFromLineHead;
	//GetLineNumber(sel->Start, &beginLineNumber, &beginPosFromLineHead);
	//GetLineNumber(sel->Start + sel->Length, &endLineNumber, &endPosFromLineHead);
	//if (beginLineNumber == endLineNumber) {
	//	m_lineSegments[beginLineNumber]->Replace(beginPosFromLineHead, sel->Length, text, len);
	//}
	//else
	//{
	//	LN_THROW(0, NotImplementedException);
	//}
}


//==============================================================================
// TextElement
//==============================================================================

//------------------------------------------------------------------------------
TextElement::TextElement()
	: m_manager(nullptr)
	, m_fontData()
	, m_foreground(nullptr)
	, m_fontDataModified(false)
	, m_position()
	, m_size(NAN, NAN)
	, m_margin()
	, m_padding()
	, m_anchor(AlignmentAnchor::None)
	, m_horizontalAlignment(HAlignment::Left)
	, m_verticalAlignment(VAlignment::Top)
	, m_parent(nullptr)
	, m_desiredSize()
	, m_finalLocalRect()
{
}

//------------------------------------------------------------------------------
TextElement::~TextElement()
{
}

//------------------------------------------------------------------------------
void TextElement::initialize()
{
	m_manager = detail::DocumentsManager::getInstance();
	m_fontData.Family = String::getEmpty();
	m_fontData.Size = 20;
	m_fontData.isBold = false;
	m_fontData.isItalic = false;
	m_fontData.isAntiAlias = true;
	m_fontDataModified = true;

	m_foreground = Brush::Black;
}

//------------------------------------------------------------------------------
Brush* TextElement::getForeground() const
{
	return m_foreground;
}

//------------------------------------------------------------------------------
void TextElement::onFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
void TextElement::render(const Matrix& transform, IDocumentsRenderer* renderer)
{
}

//------------------------------------------------------------------------------
Size TextElement::measureOverride(const Size& constraint)
{
	if (m_fontDataModified)
	{
		onFontDataChanged(m_fontData);
		m_fontDataModified = false;
	}
	return ILayoutElement::measureOverride(constraint);
}

//------------------------------------------------------------------------------
InternalTextElementType TextElement::getInternalTextElementType() const
{
	return InternalTextElementType::Common;
}

//------------------------------------------------------------------------------
const Point& TextElement::getLayoutPosition() const { return m_position; }
Size TextElement::getLayoutSize() const { return m_size; }
const Thickness& TextElement::getLayoutMargin() const { return m_margin; }
const Thickness& TextElement::getLayoutPadding() const { return m_padding; }
AlignmentAnchor TextElement::getLayoutAnchor() const { return m_anchor; }
HAlignment TextElement::getLayoutHAlignment() const { return m_horizontalAlignment; }
VAlignment TextElement::getLayoutVAlignment() const { return m_verticalAlignment; }
void TextElement::getLayoutMinMaxInfo(Size* outMin, Size* outMax) const { *outMin = Size::Zero, outMax->set(INFINITY, INFINITY); }	// TODO:
ILayoutElement* TextElement::getLayoutParent() const { return m_parent; }
const HAlignment* TextElement::getLayoutContentHAlignment() { return nullptr; }
const VAlignment* TextElement::getLayoutContentVAlignment() { return nullptr; }
const Size& TextElement::getLayoutDesiredSize() const { return m_desiredSize; }
void TextElement::setLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void TextElement::setLayoutFinalLocalRect(const Rect& renderRect) { m_finalLocalRect = renderRect; }
void TextElement::getLayoutFinalLocalRect(Rect* outRenderRect) const { LN_NOTIMPLEMENTED(); }
void TextElement::setLayoutFinalGlobalRect(const Rect& rect) { m_finalGlobalRect = rect; }
int TextElement::getVisualChildrenCount() const { return 0; }
ILayoutElement* TextElement::getVisualChild(int index) const { return nullptr; }
int TextElement::getLayoutColumn() const { return m_gridLayoutInfo.layoutColumn; }
int TextElement::getLayoutRow() const { return m_gridLayoutInfo.layoutRow; }
int TextElement::getLayoutColumnSpan() const { return m_gridLayoutInfo.layoutColumnSpan; }
int TextElement::getLayoutRowSpan() const { return m_gridLayoutInfo.layoutRowSpan; }


//==============================================================================
// Block
//==============================================================================

//------------------------------------------------------------------------------
Block::Block()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Block::~Block()
{
}

//------------------------------------------------------------------------------
void Block::initialize()
{
	TextElement::initialize();
}

//------------------------------------------------------------------------------
void Block::addInline(Inline* inl)
{
	if (LN_CHECK_ARG(inl != nullptr)) return;
	if (LN_CHECK_ARG(inl->getParent() == nullptr)) return;
	m_inlines.add(inl);
	inl->setParent(this);
}

//------------------------------------------------------------------------------
void Block::insertInlines(int index, const List<Ref<Inline>>& inlines)
{
	m_inlines.insertRange(index, inlines);
	for (Inline* inl : inlines)
	{
		inl->setParent(this);
	}
}

//------------------------------------------------------------------------------
void Block::clearInlines()
{
	for (TextElement* child : m_inlines) child->setParent(nullptr);
	m_inlines.clear();
}

//------------------------------------------------------------------------------
void Block::render(const Matrix& transform, IDocumentsRenderer* renderer)
{
	for (TextElement* child : m_inlines) child->render(transform, renderer);
}

//------------------------------------------------------------------------------
Size Block::measureOverride(const Size& constraint)
{
	Size childDesirdSize;
	for (TextElement* child : m_inlines)
	{
		// TODO: とりあえず 左から右へのフロー
		//Size size = child->measureOverride(constraint);
		child->measureLayout(constraint);
		Size size = child->getLayoutDesiredSize();
		childDesirdSize.width += size.width;
		childDesirdSize.height = std::max(childDesirdSize.height, size.height);
	}

	Size desirdSize = TextElement::measureOverride(constraint);
	return Size::max(desirdSize, childDesirdSize);
}

//------------------------------------------------------------------------------
Size Block::arrangeOverride(const Size& finalSize)
{
	float prevChildSize = 0;
	Rect childRect;
	for (TextElement* child : m_inlines)
	{
		// TODO: とりあえず 左から右へのフロー
		Size childDesiredSize = child->getDesiredSize();
		childRect.x += prevChildSize;
		prevChildSize = childDesiredSize.width;
		childRect.width = prevChildSize;
		childRect.height = std::max(childRect.height, childDesiredSize.height);
		child->arrangeLayout(childRect);
	}

	return Size::min(finalSize, childRect.getSize());
}

//==============================================================================
// Paragraph
//==============================================================================

//------------------------------------------------------------------------------
Paragraph::Paragraph()
{
}

//------------------------------------------------------------------------------
Paragraph::~Paragraph()
{
}

//------------------------------------------------------------------------------
void Paragraph::initialize()
{
	Block::initialize();
}


//==============================================================================
// Inline
//==============================================================================

//------------------------------------------------------------------------------
Inline::Inline()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Inline::~Inline()
{
}

//------------------------------------------------------------------------------
void Inline::initialize()
{
	TextElement::initialize();
}


//==============================================================================
// run
//==============================================================================

//------------------------------------------------------------------------------
run::run()
	: Inline()
{
}

//------------------------------------------------------------------------------
run::~run()
{
}

//------------------------------------------------------------------------------
void run::initialize()
{
	Inline::initialize();

	// TODO: 本当に画面に表示されている分だけ作ればいろいろ節約できそう
	m_glyphRun = Ref<GlyphRun>::makeRef();
	m_glyphRun->initialize(getManager()->getGraphicsManager());
}

//------------------------------------------------------------------------------
void run::initialize(const UTF32* str, int len)
{
	initialize();

	m_glyphRun->setText(str, len);
}

//------------------------------------------------------------------------------
void run::setText(const StringRef& text)
{
	//m_text.Clear();
	//m_text.Append(getManager()->getTCharToUTF32Converter()->convert(text.GetBegin(), text.GetLength()));
	m_glyphRun->setText(text);
}

//------------------------------------------------------------------------------
void run::onFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
Size run::measureOverride(const Size& constraint)
{
	Size size = Inline::measureOverride(constraint);
	const SizeI& runSize = m_glyphRun->getRenderSize();

	size.width = std::max(size.width, (float)runSize.width);
	size.height = std::max(size.height, (float)runSize.height);
	return size;
}

//------------------------------------------------------------------------------
void run::render(const Matrix& transform, IDocumentsRenderer* renderer)
{
	renderer->onDrawGlyphRun(transform, getForeground(), m_glyphRun, Point());
}


//==============================================================================
// LineBreak
//==============================================================================

//------------------------------------------------------------------------------
LineBreak::LineBreak()
{
}

//------------------------------------------------------------------------------
LineBreak::~LineBreak()
{
}

//------------------------------------------------------------------------------
void LineBreak::initialize()
{
	Inline::initialize();
}

//------------------------------------------------------------------------------
InternalTextElementType LineBreak::getInternalTextElementType() const
{
	return InternalTextElementType::LineBreak;
}






//==============================================================================
// VisualBlock
//==============================================================================

//------------------------------------------------------------------------------
void VisualBlock::rebuildVisualLineList()
{
	//m_visualLineList.Clear();

	//m_visualLineList.Add(newObject<VisualLine>());
	//VisualLine* lastLine = m_visualLineList.GetLast();
	//for (const Ref<TextElement>& element : m_paragraph->GetChildElements())
	//{
	//	lastLine->m_visualTextElementList.Add(newObject<VisualTextElement>());

	//	if (element->getInternalTextElementType() == InternalTextElementType::LineBreak)
	//	{
	//		m_visualLineList.Add(newObject<VisualLine>());
	//		VisualLine* lastLine = m_visualLineList.GetLast();
	//	}
	//}
}

//==============================================================================
// DocumentView
//==============================================================================

//------------------------------------------------------------------------------
void DocumentView::initialize(Document* document)
{
	m_document = document;
}

} // namespace detail
LN_NAMESPACE_END

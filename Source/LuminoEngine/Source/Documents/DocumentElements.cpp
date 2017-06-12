
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
	m_manager = detail::DocumentsManager::GetInstance();
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
	const ByteBuffer& utf32Buf = m_manager->GetTCharToUTF32Converter()->Convert(text.getBegin(), sizeof(TCHAR) * text.getLength());
	int len = utf32Buf.getSize() / sizeof(UTF32);
	replace(offset, length, (const UTF32*)utf32Buf.getConstData(), len);
}

//------------------------------------------------------------------------------
void Document::replace(int offset, int length, const UTF32* text, int len)
{
	LN_ASSERT(offset == 0 && length == 0);	// TODO: まだ

	// text を Run と LineBrake のリストにする
	List<RefPtr<Inline>> inlines;
	{
		const UTF32* pos = text;
		const UTF32* end = pos + len;
		int nlIndex = 0;
		int nlCount = 0;
		while (StringTraits::indexOfNewLineSequence(pos, end, &nlIndex, &nlCount))
		{
			inlines.add(NewObject<Run>(pos, nlIndex).get());
			inlines.add(NewObject<LineBreak>().get());
			pos += (nlIndex + nlCount);	// 改行文字の次の文字を指す
		}
		if (pos != end)
		{
			inlines.add(NewObject<Run>(pos, nlIndex).get());
		}
	}

	// TODO: Insert 先を割る
	int localInsertPoint = 0;
	LN_ASSERT(m_blockList.isEmpty());	// TODO
	RefPtr<Block> parentBlock = NewObject<Paragraph>();
	m_blockList.add(parentBlock);

	parentBlock->InsertInlines(localInsertPoint, inlines);


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
	m_manager = detail::DocumentsManager::GetInstance();
	m_fontData.Family = String::getEmpty();
	m_fontData.Size = 20;
	m_fontData.IsBold = false;
	m_fontData.IsItalic = false;
	m_fontData.IsAntiAlias = true;
	m_fontDataModified = true;

	m_foreground = Brush::Black;
}

//------------------------------------------------------------------------------
Brush* TextElement::GetForeground() const
{
	return m_foreground;
}

//------------------------------------------------------------------------------
void TextElement::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
void TextElement::Render(const Matrix& transform, IDocumentsRenderer* renderer)
{
}

//------------------------------------------------------------------------------
Size TextElement::MeasureOverride(const Size& constraint)
{
	if (m_fontDataModified)
	{
		OnFontDataChanged(m_fontData);
		m_fontDataModified = false;
	}
	return ILayoutElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
InternalTextElementType TextElement::GetInternalTextElementType() const
{
	return InternalTextElementType::Common;
}

//------------------------------------------------------------------------------
const PointF& TextElement::GetLayoutPosition() const { return m_position; }
Size TextElement::GetLayoutSize() const { return m_size; }
const ThicknessF& TextElement::GetLayoutMargin() const { return m_margin; }
const ThicknessF& TextElement::GetLayoutPadding() const { return m_padding; }
AlignmentAnchor TextElement::GetLayoutAnchor() const { return m_anchor; }
HAlignment TextElement::GetLayoutHAlignment() const { return m_horizontalAlignment; }
VAlignment TextElement::GetLayoutVAlignment() const { return m_verticalAlignment; }
void TextElement::GetLayoutMinMaxInfo(Size* outMin, Size* outMax) const { *outMin = Size::Zero, outMax->Set(INFINITY, INFINITY); }	// TODO:
ILayoutElement* TextElement::GetLayoutParent() const { return m_parent; }
const HAlignment* TextElement::GetLayoutContentHAlignment() { return nullptr; }
const VAlignment* TextElement::GetLayoutContentVAlignment() { return nullptr; }
const Size& TextElement::GetLayoutDesiredSize() const { return m_desiredSize; }
void TextElement::SetLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void TextElement::SetLayoutFinalLocalRect(const Rect& rect) { m_finalLocalRect = rect; }
const Rect& TextElement::GetLayoutFinalLocalRect() const { return m_finalLocalRect; }
void TextElement::SetLayoutFinalGlobalRect(const Rect& rect) { m_finalGlobalRect = rect; }
int TextElement::GetVisualChildrenCount() const { return 0; }
ILayoutElement* TextElement::GetVisualChild(int index) const { return nullptr; }
int TextElement::GetLayoutColumn() const { return m_gridLayoutInfo.layoutColumn; }
int TextElement::GetLayoutRow() const { return m_gridLayoutInfo.layoutRow; }
int TextElement::GetLayoutColumnSpan() const { return m_gridLayoutInfo.layoutColumnSpan; }
int TextElement::GetLayoutRowSpan() const { return m_gridLayoutInfo.layoutRowSpan; }


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
void Block::AddInline(Inline* inl)
{
	if (LN_CHECK_ARG(inl != nullptr)) return;
	if (LN_CHECK_ARG(inl->GetParent() == nullptr)) return;
	m_inlines.add(inl);
	inl->SetParent(this);
}

//------------------------------------------------------------------------------
void Block::InsertInlines(int index, const List<RefPtr<Inline>>& inlines)
{
	m_inlines.insertRange(index, inlines);
	for (Inline* inl : inlines)
	{
		inl->SetParent(this);
	}
}

//------------------------------------------------------------------------------
void Block::ClearInlines()
{
	for (TextElement* child : m_inlines) child->SetParent(nullptr);
	m_inlines.clear();
}

//------------------------------------------------------------------------------
void Block::Render(const Matrix& transform, IDocumentsRenderer* renderer)
{
	for (TextElement* child : m_inlines) child->Render(transform, renderer);
}

//------------------------------------------------------------------------------
Size Block::MeasureOverride(const Size& constraint)
{
	Size childDesirdSize;
	for (TextElement* child : m_inlines)
	{
		// TODO: とりあえず 左から右へのフロー
		//Size size = child->MeasureOverride(constraint);
		child->MeasureLayout(constraint);
		Size size = child->GetLayoutDesiredSize();
		childDesirdSize.width += size.width;
		childDesirdSize.height = std::max(childDesirdSize.height, size.height);
	}

	Size desirdSize = TextElement::MeasureOverride(constraint);
	return Size::Max(desirdSize, childDesirdSize);
}

//------------------------------------------------------------------------------
Size Block::ArrangeOverride(const Size& finalSize)
{
	float prevChildSize = 0;
	Rect childRect;
	for (TextElement* child : m_inlines)
	{
		// TODO: とりあえず 左から右へのフロー
		Size childDesiredSize = child->GetDesiredSize();
		childRect.x += prevChildSize;
		prevChildSize = childDesiredSize.width;
		childRect.width = prevChildSize;
		childRect.height = std::max(childRect.height, childDesiredSize.height);
		child->ArrangeLayout(childRect);
	}

	return Size::Min(finalSize, childRect.getSize());
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
// Run
//==============================================================================

//------------------------------------------------------------------------------
Run::Run()
	: Inline()
{
}

//------------------------------------------------------------------------------
Run::~Run()
{
}

//------------------------------------------------------------------------------
void Run::initialize()
{
	Inline::initialize();

	// TODO: 本当に画面に表示されている分だけ作ればいろいろ節約できそう
	m_glyphRun = RefPtr<GlyphRun>::makeRef();
	m_glyphRun->initialize(GetManager()->GetGraphicsManager());
}

//------------------------------------------------------------------------------
void Run::initialize(const UTF32* str, int len)
{
	initialize();

	m_glyphRun->setText(str, len);
}

//------------------------------------------------------------------------------
void Run::setText(const StringRef& text)
{
	//m_text.Clear();
	//m_text.Append(GetManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), text.GetLength()));
	m_glyphRun->setText(text);
}

//------------------------------------------------------------------------------
void Run::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
Size Run::MeasureOverride(const Size& constraint)
{
	Size size = Inline::MeasureOverride(constraint);
	const SizeI& runSize = m_glyphRun->GetRenderSize();

	size.width = std::max(size.width, (float)runSize.width);
	size.height = std::max(size.height, (float)runSize.height);
	return size;
}

//------------------------------------------------------------------------------
void Run::Render(const Matrix& transform, IDocumentsRenderer* renderer)
{
	renderer->OnDrawGlyphRun(transform, GetForeground(), m_glyphRun, PointF());
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
InternalTextElementType LineBreak::GetInternalTextElementType() const
{
	return InternalTextElementType::LineBreak;
}






//==============================================================================
// VisualBlock
//==============================================================================

//------------------------------------------------------------------------------
void VisualBlock::RebuildVisualLineList()
{
	//m_visualLineList.Clear();

	//m_visualLineList.Add(NewObject<VisualLine>());
	//VisualLine* lastLine = m_visualLineList.GetLast();
	//for (const RefPtr<TextElement>& element : m_paragraph->GetChildElements())
	//{
	//	lastLine->m_visualTextElementList.Add(NewObject<VisualTextElement>());

	//	if (element->GetInternalTextElementType() == InternalTextElementType::LineBreak)
	//	{
	//		m_visualLineList.Add(NewObject<VisualLine>());
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

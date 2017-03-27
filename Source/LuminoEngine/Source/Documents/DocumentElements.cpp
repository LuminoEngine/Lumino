
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
void Document::Initialize()
{
	m_manager = detail::DocumentsManager::GetInstance();
}

//------------------------------------------------------------------------------
void Document::SetText(const StringRef& text)
{
	m_blockList.Clear();


}

//------------------------------------------------------------------------------
void Document::Replace(int offset, int length, const StringRef& text)
{
	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetTCharToUTF32Converter()->Convert(text.GetBegin(), sizeof(TCHAR) * text.GetLength());
	int len = utf32Buf.GetSize() / sizeof(UTF32);
	Replace(offset, length, (const UTF32*)utf32Buf.GetConstData(), len);
}

//------------------------------------------------------------------------------
void Document::Replace(int offset, int length, const UTF32* text, int len)
{
	LN_ASSERT(offset == 0 && length == 0);	// TODO: まだ

	// text を Run と LineBrake のリストにする
	List<RefPtr<Inline>> inlines;
	{
		const UTF32* pos = text;
		const UTF32* end = pos + len;
		int nlIndex = 0;
		int nlCount = 0;
		while (StringTraits::IndexOfNewLineSequence(pos, end, &nlIndex, &nlCount))
		{
			inlines.Add(NewObject<Run>(pos, nlIndex).Get());
			inlines.Add(NewObject<LineBreak>().Get());
			pos += (nlIndex + nlCount);	// 改行文字の次の文字を指す
		}
		if (pos != end)
		{
			inlines.Add(NewObject<Run>(pos, nlIndex).Get());
		}
	}

	// TODO: Insert 先を割る
	int localInsertPoint = 0;
	LN_ASSERT(m_blockList.IsEmpty());	// TODO
	RefPtr<Block> parentBlock = NewObject<Paragraph>();
	m_blockList.Add(parentBlock);

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
void TextElement::Initialize()
{
	m_manager = detail::DocumentsManager::GetInstance();
	m_fontData.Family = String::GetEmpty();
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
ILayoutElement* TextElement::GetLayoutParent() const { return m_parent; }
const HAlignment* TextElement::GetLayoutContentHAlignment() { return nullptr; }
const VAlignment* TextElement::GetLayoutContentVAlignment() { return nullptr; }
const Size& TextElement::GetLayoutDesiredSize() const { return m_desiredSize; }
void TextElement::SetLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void TextElement::SetLayoutFinalLocalRect(const RectF& rect) { m_finalLocalRect = rect; }
const RectF& TextElement::GetLayoutFinalLocalRect() const { return m_finalLocalRect; }
void TextElement::SetLayoutFinalGlobalRect(const RectF& rect) { m_finalGlobalRect = rect; }
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
void Block::Initialize()
{
	TextElement::Initialize();
}

//------------------------------------------------------------------------------
void Block::AddInline(Inline* inl)
{
	if (LN_CHECK_ARG(inl != nullptr)) return;
	if (LN_CHECK_ARG(inl->GetParent() == nullptr)) return;
	m_inlines.Add(inl);
	inl->SetParent(this);
}

//------------------------------------------------------------------------------
void Block::InsertInlines(int index, const List<RefPtr<Inline>>& inlines)
{
	m_inlines.InsertRange(index, inlines);
	for (Inline* inl : inlines)
	{
		inl->SetParent(this);
	}
}

//------------------------------------------------------------------------------
void Block::ClearInlines()
{
	for (TextElement* child : m_inlines) child->SetParent(nullptr);
	m_inlines.Clear();
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
	RectF childRect;
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

	return Size::Min(finalSize, childRect.GetSize());
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
void Paragraph::Initialize()
{
	Block::Initialize();
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
void Inline::Initialize()
{
	TextElement::Initialize();
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
void Run::Initialize()
{
	Inline::Initialize();

	// TODO: 本当に画面に表示されている分だけ作ればいろいろ節約できそう
	m_glyphRun = RefPtr<GlyphRun>::MakeRef();
	m_glyphRun->Initialize(GetManager()->GetGraphicsManager());
}

//------------------------------------------------------------------------------
void Run::Initialize(const UTF32* str, int len)
{
	Initialize();

	m_glyphRun->SetText(str, len);
}

//------------------------------------------------------------------------------
void Run::SetText(const StringRef& text)
{
	//m_text.Clear();
	//m_text.Append(GetManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), text.GetLength()));
	m_glyphRun->SetText(text);
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
	renderer->OnDrawGlyphRun(transform, GetForeground(), m_glyphRun, PointF::Zero);
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
void LineBreak::Initialize()
{
	Inline::Initialize();
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
void DocumentView::Initialize(Document* document)
{
	m_document = document;
}

} // namespace detail
LN_NAMESPACE_END

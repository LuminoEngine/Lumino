
#include "Internal.h"
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "DocumentsManager.h"
#include <Lumino/Documents/Documents.h>

LN_NAMESPACE_BEGIN
namespace tr {

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
	m_manager = ln::detail::DocumentsManager::GetInstance();
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
{
}

//------------------------------------------------------------------------------
TextElement::~TextElement()
{
}

//------------------------------------------------------------------------------
void TextElement::Initialize()
{
	m_manager = ln::detail::DocumentsManager::GetInstance();
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
void TextElement::OnFontDataChanged(const ln::detail::FontData& newData)
{
}


//------------------------------------------------------------------------------
InternalTextElementType TextElement::GetInternalTextElementType() const
{
	return InternalTextElementType::Common;
}


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
	LN_CHECK_ARG(inl != nullptr);
	LN_CHECK_ARG(inl->GetParent() == nullptr);
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
void Run::OnFontDataChanged(const ln::detail::FontData& newData)
{
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
void VisualBlock::SetBlock(Block* block)
{
	m_block = block;
}

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

//------------------------------------------------------------------------------
void DocumentView::Render(const Matrix& transform, ln::detail::IDocumentsRenderer* renderer)
{

}

} // namespace tr
LN_NAMESPACE_END

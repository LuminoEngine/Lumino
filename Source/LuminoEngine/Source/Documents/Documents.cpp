
#include "Internal.h"
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Rendering.h>
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

	Replace(0, 0, text);
}

//------------------------------------------------------------------------------
void Document::Replace(int offset, int length, const StringRef& text)
{
	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetTCharToUTF32Converter()->Convert(text.GetBegin(), sizeof(TCHAR) * text.GetLength());
	int len = utf32Buf.GetSize() / sizeof(UTF32);
	ReplaceInternal(offset, length, (const UTF32*)utf32Buf.GetConstData(), len);
}

//------------------------------------------------------------------------------
void Document::ReplaceInternal(int offset, int length, const UTF32* text, int len)
{
	LN_ASSERT(offset == 0 && length == 0);	// TODO: まだ

	// text を Run と LineBreak のリストにする
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
			inlines.Add(NewObject<Run>(pos, end - pos).Get());
		}
	}

	// TODO: Insert 先を割る
	int localInsertPoint = 0;
	LN_ASSERT(m_blockList.IsEmpty());	// TODO
	RefPtr<Block> parentBlock = NewObject<Paragraph>();
	m_blockList.Add(parentBlock);
	parentBlock->SetParentContent(this);
	IncreaseRevision();

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
	//, m_thisRevision(0)
	//, m_childrenRevision(0)
	, m_deleted(false)
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
	if (LN_CHECK_ARG(inl != nullptr)) return;
	m_inlines.Add(inl);
	inl->SetParentContent(this);
	IncreaseRevision();
}

//------------------------------------------------------------------------------
void Block::InsertInlines(int index, const List<RefPtr<Inline>>& inlines)
{
	m_inlines.InsertRange(index, inlines);
	for (Inline* inl : inlines)
	{
		inl->SetParentContent(this);
	}
	IncreaseRevision();
}

//------------------------------------------------------------------------------
void Block::ClearInlines()
{
	for (TextElement* child : m_inlines) child->SetParentContent(nullptr);
	m_inlines.Clear();
	IncreaseRevision();
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
	//m_glyphRun = RefPtr<GlyphRun>::MakeRef();
	//m_glyphRun->Initialize(GetManager()->GetGraphicsManager());
}

//------------------------------------------------------------------------------
void Run::Initialize(const UTF32* str, int len)
{
	Initialize();

	m_text.Clear();
	m_text.Append(str, len/*GetManager()->GetTCharToUTF32Converter()->Convert(str, len)*/);
	IncreaseRevision();
}

////------------------------------------------------------------------------------
//void Run::SetText(const StringRef& text)
//{
//	m_text.Clear();
//	m_text.Append(GetManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), text.GetLength()));
//	//m_glyphRun->SetText(text);
//
//	IncreaseRevision();
//}

//------------------------------------------------------------------------------
void Run::OnFontDataChanged(const ln::detail::FontData& newData)
{
}

//------------------------------------------------------------------------------
InternalTextElementType Run::GetInternalTextElementType() const
{
	return InternalTextElementType::TextRun;
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











/*
	1. Block::Measure()
		m_inlines を階層的に Measure する。
	2. Inline(Run)::Measure()
		カーニングを考慮して、VisualGlyph をたくさん作る。ここは TextLayoutEngine 使える。
		ついでにルートの Block の visualGlyph リストへ追加していく。
		変数の展開やルビの配置はここ。
	2. Inline(Run以外)::Measure()
		画像を示す VisualGlyph を作る。
	3. Block::Measure() (2. の呼び出しから戻ってきたとき)
		折り返しは考慮しないサイズで desiardSize を決定する。これの height が行高さとなる。（余白・ルビ考慮）
	4. Block::Arrange()
		折り返しが必要ならここで VisualGlyph たちの位置を調整する。（ここまできたらもう Inline は関係ない）

	### ルビ
	配置は単に VisualGlyph のリストに突っ込んでok。
	折り返し対応するとなったら、単語的なグループ化が必要。
*/


//==============================================================================
// VisualTextElement
//==============================================================================

//------------------------------------------------------------------------------
VisualGlyph::VisualGlyph()
{
}

//------------------------------------------------------------------------------
VisualGlyph::~VisualGlyph()
{
}

//------------------------------------------------------------------------------
void VisualGlyph::Initialize()
{
}

//------------------------------------------------------------------------------
void VisualGlyph::Render(DrawList* renderer)
{
	renderer->SetBrush(Brush::Red);
	renderer->DrawRectangle(m_localRect);
}

//==============================================================================
// VisualTextElement
//==============================================================================

//------------------------------------------------------------------------------
VisualTextElement::VisualTextElement()
	//: m_thisRevision(0)
	//, m_childrenRevision(0)
{
}

//------------------------------------------------------------------------------
VisualTextElement::~VisualTextElement()
{
}




//==============================================================================
// VisualInline
//==============================================================================

//------------------------------------------------------------------------------
VisualInline::VisualInline()
{
}

//------------------------------------------------------------------------------
VisualInline::~VisualInline()
{
}

//------------------------------------------------------------------------------
void VisualInline::Initialize(Inline* inl)
{
	m_inline = inl;
}

//------------------------------------------------------------------------------
void VisualInline::MeasureLayout(const Size& availableSize, VisualBlock* rootBlock)
{
	// update this
	if (GetThisRevision() != m_inline->GetThisRevision())
	{
		// Model が Run なら GlyphRun を作っておく
		if (m_inline->GetInternalTextElementType() == InternalTextElementType::TextRun)
		{
			if (m_glyphRun == nullptr)
			{
				m_glyphRun = RefPtr<GlyphRun>::MakeRef();
				m_glyphRun->Initialize(ln::detail::EngineDomain::GetGraphicsManager());
			}

			auto* run = static_cast<Run*>(m_inline.Get());
			m_glyphRun->SetText(run->GetText(), run->GetLength());
		}

		SetThisRevision(m_inline->GetThisRevision());
	}

	// Block 下の Inline のうち1つでも変更があれば、Block 下の全ての Inline は再更新が必要になる
	if (m_glyphRun != nullptr)
	{
		auto& items = m_glyphRun->RequestLayoutItems();
		for (auto& item : items)
		{
			auto g = NewObject<VisualGlyph>();	// TODO: キャッシュしたい
			g->m_localRect.Set(
				item.Location.BitmapTopLeftPosition.x,
				item.Location.BitmapTopLeftPosition.y,
				item.Location.BitmapSize.width,
				item.Location.BitmapSize.height);
			rootBlock->AddVisualGlyph(g);
		}
	}

	// update children
	if (GetChildrenRevision() != m_inline->GetChildrenRevision())
	{
		// TODO: 必要ないかも
	}




	
}

////------------------------------------------------------------------------------
//void VisualInline::ArrangeLayout(const RectF& finalLocalRect)
//{
//}
//
////------------------------------------------------------------------------------
//void VisualInline::Render(const Matrix& transform, ln::detail::IDocumentsRenderer* renderer)
//{
//}

//==============================================================================
// VisualBlock
//==============================================================================

//------------------------------------------------------------------------------
VisualBlock::VisualBlock()
{
}

//------------------------------------------------------------------------------
VisualBlock::~VisualBlock()
{
}

//------------------------------------------------------------------------------
void VisualBlock::Initialize(Block* block)
{
	m_block = block;
}

//------------------------------------------------------------------------------
void VisualBlock::SetBlock(Block* block)
{
	m_block = block;
}

//------------------------------------------------------------------------------
bool VisualBlock::IsModelDeleted() const
{
	return m_block->IsDeleted();
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

//------------------------------------------------------------------------------
void VisualBlock::MeasureLayout(const Size& availableSize)
{
	// update this (inline list)
	if (GetThisRevision() != m_block->GetThisRevision())
	{
		LN_ASSERT(GetThisRevision() == 0);	// TODO: 今は初回のみ

		for (auto& inl : m_block->GetInlines())
		{
			m_visualInlines.Add(NewObject<VisualInline>(inl));
		}

		SetThisRevision(m_block->GetThisRevision());
	}

	// update children
	if (GetChildrenRevision() != m_block->GetChildrenRevision())
	{
		m_visualGlyphs.Clear();

		for (auto& inl : m_visualInlines)
		{
			inl->MeasureLayout(availableSize, this);
		}

		SetChildrenRevision(m_block->GetChildrenRevision());
	}
}

//------------------------------------------------------------------------------
void VisualBlock::ArrangeLayout(const RectF& finalLocalRect)
{
}

//------------------------------------------------------------------------------
void VisualBlock::Render(DrawList* renderer)
{
	for (auto& glyph : m_visualGlyphs)
	{
		glyph->Render(renderer);
	}
}


//==============================================================================
// DocumentView
//------------------------------------------------------------------------------
/*
	Visual 側は、Model の参照を持つ。IsDeleted() = true だったら、Measure で消す。
*/
//==============================================================================

//------------------------------------------------------------------------------
DocumentView::DocumentView()
	//: m_thisRevision(0)
	//, m_childrenRevision(0)
{
}

//------------------------------------------------------------------------------
DocumentView::~DocumentView()
{
}

//------------------------------------------------------------------------------
void DocumentView::Initialize(Document* document)
{
	m_document = document;
}

//------------------------------------------------------------------------------
void DocumentView::MeasureLayout(const Size& availableSize)
{
	// Delete 済みの Model を持つ Block を全て取り除く
	//m_visualBlicks.RemoveAll([](const RefPtr<VisualBlock>& ptr) { return ptr->IsModelDeleted(); });

	// update this (block list)
	if (GetThisRevision() != m_document->GetThisRevision())
	{
		LN_ASSERT(GetThisRevision() == 0);	// TODO: 今は初回のみ

		for (auto& block : m_document->GetBlocks())
		{
			m_visualBlocks.Add(NewObject<VisualBlock>(block));
		}

		SetThisRevision(m_document->GetThisRevision());
	}

	// update children
	if (GetChildrenRevision() != m_document->GetChildrenRevision())
	{
		for (auto& block : m_visualBlocks)
		{
			block->MeasureLayout(availableSize);
		}

		SetChildrenRevision(m_document->GetChildrenRevision());
	}
}

//------------------------------------------------------------------------------
void DocumentView::ArrangeLayout(const RectF& finalLocalRect)
{
}

//------------------------------------------------------------------------------
void DocumentView::Render(DrawList* renderer)
{
	for (auto& block : m_visualBlocks)
	{
		block->Render(renderer);
	}
}

} // namespace tr
LN_NAMESPACE_END

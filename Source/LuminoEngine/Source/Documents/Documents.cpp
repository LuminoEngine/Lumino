
#include "Internal.h"
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include <Lumino/Rendering/Rendering.h>
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
void Document::initialize()
{
	m_manager = ln::detail::DocumentsManager::getInstance();
}

//------------------------------------------------------------------------------
void Document::setText(const StringRef& text)
{
	m_blockList.clear();

	replace(0, 0, text);
}

//------------------------------------------------------------------------------
void Document::replace(int offset, int length, const StringRef& text)
{
	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->getTCharToUTF32Converter()->convert(text.getBegin(), sizeof(Char) * text.getLength());
	int len = utf32Buf.getSize() / sizeof(UTF32);
	replaceInternal(offset, length, (const UTF32*)utf32Buf.getConstData(), len);
}

//------------------------------------------------------------------------------
void Document::replaceInternal(int offset, int length, const UTF32* text, int len)
{
	LN_ASSERT(offset == 0 && length == 0);	// TODO: まだ

	// text を run と LineBreak のリストにする
	List<Ref<Inline>> inlines;
	{
		const UTF32* pos = text;
		const UTF32* end = pos + len;
		int nlIndex = 0;
		int nlCount = 0;
		while (StringTraits::indexOfNewLineSequence(pos, end, &nlIndex, &nlCount))
		{
			inlines.add(newObject<Run>(pos, nlIndex).get());
			inlines.add(newObject<LineBreak>().get());
			pos += (nlIndex + nlCount);	// 改行文字の次の文字を指す
		}
		if (pos != end)
		{
			inlines.add(newObject<Run>(pos, end - pos).get());
		}
	}

	// TODO: Insert 先を割る
	int localInsertPoint = 0;
	LN_ASSERT(m_blockList.isEmpty());	// TODO
	Ref<Block> parentBlock = newObject<Paragraph>();
	m_blockList.add(parentBlock);
	parentBlock->setParentContent(this);
	increaseRevision();

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
void TextElement::initialize()
{
	m_manager = ln::detail::DocumentsManager::getInstance();
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
void TextElement::onFontDataChanged(const ln::detail::FontData& newData)
{
}


//------------------------------------------------------------------------------
InternalTextElementType TextElement::getInternalTextElementType() const
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
void Block::initialize()
{
	TextElement::initialize();
}

//------------------------------------------------------------------------------
void Block::addInline(Inline* inl)
{
	if (LN_CHECK_ARG(inl != nullptr)) return;
	m_inlines.add(inl);
	inl->setParentContent(this);
	increaseRevision();
}

//------------------------------------------------------------------------------
void Block::insertInlines(int index, const List<Ref<Inline>>& inlines)
{
	m_inlines.insertRange(index, inlines);
	for (Inline* inl : inlines)
	{
		inl->setParentContent(this);
	}
	increaseRevision();
}

//------------------------------------------------------------------------------
void Block::clearInlines()
{
	for (TextElement* child : m_inlines) child->setParentContent(nullptr);
	m_inlines.clear();
	increaseRevision();
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
	//m_glyphRun = Ref<GlyphRun>::MakeRef();
	//m_glyphRun->initialize(getManager()->getGraphicsManager());
}

//------------------------------------------------------------------------------
void Run::initialize(const UTF32* str, int len)
{
	initialize();

	m_text.clear();
	m_text.append(str, len/*getManager()->getTCharToUTF32Converter()->convert(str, len)*/);
	increaseRevision();
}

////------------------------------------------------------------------------------
//void Run::SetText(const StringRef& text)
//{
//	m_text.Clear();
//	m_text.Append(getManager()->getTCharToUTF32Converter()->convert(text.GetBegin(), text.GetLength()));
//	//m_glyphRun->SetText(text);
//
//	increaseRevision();
//}

//------------------------------------------------------------------------------
void Run::onFontDataChanged(const ln::detail::FontData& newData)
{
}

//------------------------------------------------------------------------------
InternalTextElementType Run::getInternalTextElementType() const
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
void LineBreak::initialize()
{
	Inline::initialize();
}

//------------------------------------------------------------------------------
InternalTextElementType LineBreak::getInternalTextElementType() const
{
	return InternalTextElementType::LineBreak;
}











/*
	1. Block::measure()
		m_inlines を階層的に measure する。
	2. Inline(run)::measure()
		カーニングを考慮して、VisualGlyph をたくさん作る。ここは TextLayoutEngine 使える。
		ついでにルートの Block の visualGlyph リストへ追加していく。
		変数の展開やルビの配置はここ。
	2. Inline(run以外)::measure()
		画像を示す VisualGlyph を作る。
	3. Block::measure() (2. の呼び出しから戻ってきたとき)
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

////------------------------------------------------------------------------------
//VisualGlyph::VisualGlyph()
//{
//}
//
////------------------------------------------------------------------------------
//VisualGlyph::~VisualGlyph()
//{
//}
//
////------------------------------------------------------------------------------
//void VisualGlyph::initialize()
//{
//}
//
////------------------------------------------------------------------------------
//void VisualGlyph::render(DrawList* renderer)
//{
//	renderer->setBrush(Brush::Red);
//	renderer->drawRectangle(m_localRect);
//}


//==============================================================================
// VisualTextFragment
//==============================================================================

//------------------------------------------------------------------------------
VisualTextFragment::VisualTextFragment()
{
}

//------------------------------------------------------------------------------
VisualTextFragment::~VisualTextFragment()
{
}

//------------------------------------------------------------------------------
void VisualTextFragment::initialize()
{
}

//------------------------------------------------------------------------------
void VisualTextFragment::render(DrawList* renderer)
{
	renderer->setBrush(Brush::Red);
	//renderer->drawRectangle(m_localRect);
	renderer->drawGlyphRun(Point(), m_glyphRun);
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
void VisualInline::initialize(Inline* inl)
{
	m_inline = inl;
}

//------------------------------------------------------------------------------
void VisualInline::measureLayout(const Size& availableSize, VisualBlock* rootBlock)
{
	// update this
	// Block 下の Inline のうち1つでも変更があれば、Block 下の全ての Inline は再更新が必要になる
	if (getThisRevision() != m_inline->getThisRevision())
	{
		// Model が run なら GlyphRun を作っておく
		if (m_inline->getInternalTextElementType() == InternalTextElementType::TextRun)
		{
			//if (m_glyphRun == nullptr)
			{
				auto frag = newObject<VisualTextFragment>();	// TODO: キャッシュしたい
				frag->m_glyphRun = Ref<GlyphRun>::makeRef();
				frag->m_glyphRun->initialize(ln::detail::EngineDomain::getGraphicsManager());

				auto* run = static_cast<Run*>(m_inline.get());
				frag->m_glyphRun->setText(run->getText(), run->getLength());

				rootBlock->addVisualFragment(frag);
			}

		}

		setThisRevision(m_inline->getThisRevision());
	}

	//if (m_glyphRun != nullptr)
	//{
	//	auto& items = m_glyphRun->requestLayoutItems();
	//	for (auto& item : items)
	//	{
	//		auto g = newObject<VisualTextFragment>();	// TODO: キャッシュしたい
	//		g->m_localRect.Set(
	//			item.Location.BitmapTopLeftPosition.x,
	//			item.Location.BitmapTopLeftPosition.y,
	//			item.Location.BitmapSize.width,
	//			item.Location.BitmapSize.height);
	//		rootBlock->addVisualFragment(g);
	//	}
	//}

	// update children
	if (getChildrenRevision() != m_inline->getChildrenRevision())
	{
		// TODO: 必要ないかも
	}




	
}

////------------------------------------------------------------------------------
//void VisualInline::arrangeLayout(const Rect& finalLocalRect)
//{
//}
//
////------------------------------------------------------------------------------
//void VisualInline::render(const Matrix& transform, ln::detail::IDocumentsRenderer* renderer)
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
void VisualBlock::initialize(Block* block)
{
	m_block = block;
}

//------------------------------------------------------------------------------
void VisualBlock::setBlock(Block* block)
{
	m_block = block;
}

//------------------------------------------------------------------------------
bool VisualBlock::isModelDeleted() const
{
	return m_block->isDeleted();
}

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

//------------------------------------------------------------------------------
void VisualBlock::measureLayout(const Size& availableSize)
{
	// update this (inline list)
	if (getThisRevision() != m_block->getThisRevision())
	{
		LN_ASSERT(getThisRevision() == 0);	// TODO: 今は初回のみ

		for (auto& inl : m_block->getInlines())
		{
			m_visualInlines.add(newObject<VisualInline>(inl));
		}

		setThisRevision(m_block->getThisRevision());
	}

	// update children
	if (getChildrenRevision() != m_block->getChildrenRevision())
	{
		m_visualFragments.clear();

		for (auto& inl : m_visualInlines)
		{
			inl->measureLayout(availableSize, this);
		}

		setChildrenRevision(m_block->getChildrenRevision());
	}
}

//------------------------------------------------------------------------------
void VisualBlock::arrangeLayout(const Rect& finalLocalRect)
{
}

//------------------------------------------------------------------------------
void VisualBlock::render(DrawList* renderer)
{
	for (auto& glyph : m_visualFragments)
	{
		glyph->render(renderer);
	}
}


//==============================================================================
// DocumentView
//------------------------------------------------------------------------------
/*
	Visual 側は、Model の参照を持つ。isDeleted() = true だったら、measure で消す。
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
void DocumentView::initialize(Document* document)
{
	m_document = document;
}

//------------------------------------------------------------------------------
void DocumentView::measureLayout(const Size& availableSize)
{
	// Delete 済みの Model を持つ Block を全て取り除く
	//m_visualBlicks.RemoveAll([](const Ref<VisualBlock>& ptr) { return ptr->isModelDeleted(); });

	// update this (block list)
	if (getThisRevision() != m_document->getThisRevision())
	{
		LN_ASSERT(getThisRevision() == 0);	// TODO: 今は初回のみ

		for (auto& block : m_document->getBlocks())
		{
			m_visualBlocks.add(newObject<VisualBlock>(block));
		}

		setThisRevision(m_document->getThisRevision());
	}

	// update children
	if (getChildrenRevision() != m_document->getChildrenRevision())
	{
		for (auto& block : m_visualBlocks)
		{
			block->measureLayout(availableSize);
		}

		setChildrenRevision(m_document->getChildrenRevision());
	}
}

//------------------------------------------------------------------------------
void DocumentView::arrangeLayout(const Rect& finalLocalRect)
{
}

//------------------------------------------------------------------------------
void DocumentView::render(DrawList* renderer)
{
	for (auto& block : m_visualBlocks)
	{
		block->render(renderer);
	}
}

} // namespace tr
LN_NAMESPACE_END

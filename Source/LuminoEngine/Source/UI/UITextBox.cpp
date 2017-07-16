
#include "Internal.h"
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include <Lumino/Documents/Documents.h>
#include <Lumino/UI/UITextBox.h>
#include "UIManager.h"

#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Text/FontManager.h"

LN_NAMESPACE_BEGIN

class UITextDocumentLine
	//: public tr::DocumentContentElement
	: public Object
{
public:
	enum class LineDelimiter
	{
		CR,
		LF,
		CRLF,
		EndOfFile,
	};

	static const UTF32 EofCode = '\0';
	static const UTF32 NlCode = '\n';

	int getLength() const { return m_text.getLength()/* + 1*/; }	//// LineDelimiter 分の +1
	bool isEmpty() const { return m_text.isEmpty(); }

LN_CONSTRUCT_ACCESS:
	UITextDocumentLine();
	virtual ~UITextDocumentLine();
	void initialize();
	void initialize(const UTF32* text, int len, UTF32 lineDelim);

LN_INTERNAL_ACCESS:
	void removeInternal(int offset, int length);
	void insertText(int offset, const UTF32* text, int len);
	//void AppendText(const UTF32* text, int len);
	void setLineDelimiter(LineDelimiter delim) { m_lineDelimiter = delim; }
	UTF32 getLineDelimiterCode() const { return m_text.c_str()[m_text.getLength() - 1]; }
	const UTF32* getText() const { return m_text.c_str(); }
	int getTextLength2() const { return m_text.getLength() - 1; }
	void clear();
	int getRevision() const { return m_revision; }

private:
	GenericStringBuilderCore<UTF32>	m_text;				// \n を含む
	LineDelimiter					m_lineDelimiter;
	int								m_revision;
};

class UITextDocument
	: public Object//tr::DocumentContentElement
{
public:
	void replace(int offset, int length, const StringRef& text);

LN_CONSTRUCT_ACCESS:
	UITextDocument();
	virtual ~UITextDocument();
	void initialize();

LN_INTERNAL_ACCESS:
	int getRevision() const { return m_revision; }
	const List<Ref<UITextDocumentLine>>& GetLines() const { return m_lines; }

private:
	void replaceInternal(int offset, int length, const UTF32* text, int len);
	void removeInternal(int offset, int length);
	void insertInternal(int offset, const UTF32* text, int len);
	int findLineIndexFromOffset(int offset, int* outOffsetFromLineHead = nullptr);	// 0~
	UITextDocumentLine* findLineFromOffset(int offset);

	detail::UIManager*					m_manager;
	List<Ref<UITextDocumentLine>>	m_lines;		// 必ず1つ入っている。
	int									m_revision;
};





struct UITextVisualPosition
{
	int	line;
	int column;
};


// 物理行
class UITextVisualLine
	: public Object
{
public:
	const Size& getRenderSize() const { return m_renderRect.getSize(); }
	const Rect& getRenderRect() const { return m_renderRect; }

LN_CONSTRUCT_ACCESS:
	UITextVisualLine();
	virtual ~UITextVisualLine();
	void initialize();

LN_INTERNAL_ACCESS:
	void setStartDocumentTextOffset(int offset) { m_startDocumentTextOffset = offset; }
	int getStartDocumentTextOffset() const { return  m_startDocumentTextOffset; }
	void addGlyphRun(GlyphRun* run);
	void setVisualLineIndex(int index) { m_visualLineIndex = index; }
	void setRenderPt(const PointF& pt) { m_renderRect.setLocation(pt); }
	void render(DrawingContext* g);
	Rect getGlyphAreaGlobalRect(int column);	// 高さ=行高さ
	bool testHitFromGlobalPoint(const PointF& pt, UITextVisualPosition* outPos);
	int getTextLength() const { return m_textLength; }

private:
	List<Ref<GlyphRun>>	m_glyphRuns;
	int						m_startDocumentTextOffset;
	int						m_textLength;		// LineDelim も含む。ただし、折り返しなどでの改行には含まない
	int						m_visualLineIndex;
	Rect					m_renderRect;	// TextArea 座標系
	//float					m_renderTop;
	//Size					m_renderSize;
};

// 物理行のまとまり
class UITextVisualLineBlock
	: public Object
{
public:
	bool isModified() const;
	UITextDocumentLine* getDocumentLine() const { return m_documentLine; }

	int getVisualLinesCount() const { return m_visualLines.getCount(); }
	UITextVisualLine* getVisualLine(int index) const { return m_visualLines[index]; }

	//float GetRenderTop() const { return m_renderTop; }
	//float GetRenderHeight() const { return m_renderSize.width; }
	const Rect& getRenderRect() const { return m_renderRect; }

	void buildVisualLines(Font* font, int startDocumentTextOffset, int visualLineIndex, const PointF& renderPt);

LN_CONSTRUCT_ACCESS:
	UITextVisualLineBlock();
	virtual ~UITextVisualLineBlock();
	void initialize(UITextDocumentLine* documentLine);

LN_INTERNAL_ACCESS:
	void render(DrawingContext* g);
	const List<Ref<UITextVisualLine>>& getVisualLines() const { return m_visualLines; }

private:
	Ref<UITextDocumentLine>		m_documentLine;
	int								m_revision;
	List<Ref<UITextVisualLine>>	m_visualLines;
	Rect							m_renderRect;	// TextArea 座標系
	//float							m_renderTop;
	//Size							m_renderSize;
};




class UITextAreaCaret
	: public Object
{
public:
	void setVisualPosition(const UITextVisualPosition& pos) { m_position = pos; }

LN_CONSTRUCT_ACCESS:
	UITextAreaCaret();
	virtual ~UITextAreaCaret();
	void initialize();

LN_INTERNAL_ACCESS:
	const UITextVisualPosition& getVisualPosition() const { return m_position; }
	void setRenderRectangle(const Rect& rect) { m_renderRectangle = rect; }
	const Rect& getRenderRectangle() const { return m_renderRectangle; }

private:
	UITextVisualPosition	m_position;
	Rect					m_renderRectangle;
};


class UITextArea
	: public Object
{

public:
	UITextDocument* getDocument() const { return m_document; }
	UITextAreaCaret* getCaret() const { return m_caret; }

	Size measure(const Size& availableSize, Font* font);
	Size Arrange(const Size& finalSize);
	void render(DrawingContext* g);

LN_CONSTRUCT_ACCESS:
	UITextArea();
	virtual ~UITextArea();
	void initialize();

LN_INTERNAL_ACCESS:
	UITextVisualLine* findVisualLine(int lineIndex);
	Rect getGlyphGlobalRectFromVisualPosition(const UITextVisualPosition& pos);
	void getVisualPositionFromRenderPosition(const PointF& pt, UITextVisualPosition* outPos);
	int getDocumentTextOffset(const UITextVisualPosition& pos);

private:
	Ref<UITextDocument>				m_document;
	List<Ref<UITextVisualLineBlock>>	m_visualLineBlocks;
	int									m_revision;
	Ref<UITextAreaCaret>				m_caret;
};




//==============================================================================
// UITextDocumentLine
//==============================================================================

//------------------------------------------------------------------------------
UITextDocumentLine::UITextDocumentLine()
	: m_text()
	, m_lineDelimiter(LineDelimiter::EndOfFile)
	, m_revision(0)
{
}

//------------------------------------------------------------------------------
UITextDocumentLine::~UITextDocumentLine()
{
}

//------------------------------------------------------------------------------
void UITextDocumentLine::initialize()
{
	clear();
}

//------------------------------------------------------------------------------
void UITextDocumentLine::initialize(const UTF32* text, int len, UTF32 lineDelim)
{
	m_text.append(text, len);
	m_text.append(lineDelim);
	m_revision++;
}

//------------------------------------------------------------------------------
void UITextDocumentLine::removeInternal(int offset, int length)
{
	// adjust range
	int rangeBegin = offset;
	int rangeEnd = offset + length;
	if (rangeBegin < 0) rangeBegin = 0;
	if (rangeEnd > m_text.getLength()) rangeEnd = m_text.getLength();

	UTF32 empty[] = { 0 };
	m_text.replace(rangeBegin, rangeEnd - rangeBegin, empty, 0);	// TODO: 普通に Remove 関数とか作ったほうがパフォーマンスよさそう
	m_revision++;
}

//------------------------------------------------------------------------------
void UITextDocumentLine::insertText(int offset, const UTF32* text, int len)
{
	m_text.replace(offset, 0, text, len);	// TODO: 普通に Insert 関数とか作ったほうがパフォーマンスよさそう
	m_revision++;
}

//------------------------------------------------------------------------------
//void UITextDocumentLine::AppendText(const UTF32* text, int len)
//{
//	m_text.Append(text, len);
//}

//------------------------------------------------------------------------------
void UITextDocumentLine::clear()
{
	m_text.clear();
	m_text.append(EofCode);
	m_revision++;
}

//==============================================================================
// UITextDocument
//==============================================================================

//------------------------------------------------------------------------------
UITextDocument::UITextDocument()
	: m_revision(0)
{
}

//------------------------------------------------------------------------------
UITextDocument::~UITextDocument()
{
}

//------------------------------------------------------------------------------
void UITextDocument::initialize()
{
	m_manager = detail::EngineDomain::getUIManager();

	m_lines.add(newObject<UITextDocumentLine>());	// 空line。EOF。	// TODO: cache
	m_revision++;
}

//------------------------------------------------------------------------------
void UITextDocument::replace(int offset, int length, const StringRef& text)
{
	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->getGraphicsManager()->getFontManager()->getTCharToUTF32Converter()->convert(text.getBegin(), sizeof(TCHAR) * text.getLength());
	int len = utf32Buf.getSize() / sizeof(UTF32);
	replaceInternal(offset, length, (const UTF32*)utf32Buf.getConstData(), len);
}

//------------------------------------------------------------------------------
void UITextDocument::replaceInternal(int offset, int length, const UTF32* text, int len)
{
	removeInternal(offset, length);
	insertInternal(offset, text, len);
}

//------------------------------------------------------------------------------
void UITextDocument::removeInternal(int offset, int length)
{
	int offsetFromLineHead;
	int index = findLineIndexFromOffset(offset, &offsetFromLineHead);
	int beginOffset = offset;
	int endOffset = offset + length;
	while (beginOffset < endOffset)
	{
		int removeLength = endOffset - beginOffset;
		removeLength = std::min(removeLength, m_lines[index]->getLength());

		m_lines[index]->removeInternal(offsetFromLineHead, removeLength);

		beginOffset += removeLength;
		offsetFromLineHead = 0;
		index++;
	}

	// GC empty lines
	for (int i = m_lines.getCount() - 1; i >= 0; i--)
	{
		if (m_lines[i]->isEmpty())
		{
			if (m_lines.getCount() == 1)
			{
				// 最後の1つなら Remove はせず、空行にする
				m_lines[i]->clear();
			}
			else
			{
				m_lines.removeAt(i);
			}
			m_revision++;
		}
	}
}

//------------------------------------------------------------------------------
void UITextDocument::insertInternal(int offset, const UTF32* text, int len)
{
	//UITextDocumentLine* line = findLineFromOffset(offset);
	//if (line != nullptr)
	//{
	//	m_lines.Add(newObject<UITextDocumentLine>());	// TODO: cache
	//	line = m_lines.GetLast();
	//}

	int offsetFromLineHead = 0;
	int lineInsertIndex = findLineIndexFromOffset(offset, &offsetFromLineHead);
	//if (lineInsertIndex < 0)
	//{
	//	m_lines.Add(newObject<UITextDocumentLine>());	// 空line。EOF。	// TODO: cache
	//	lineInsertIndex = 0;
	//}
	Ref<UITextDocumentLine> firstLine = m_lines[lineInsertIndex];

	// まず全部 Insert する
	firstLine->insertText(offsetFromLineHead, text, len);
	






	// 改行が含まれているか？
	const UTF32* begin = firstLine->getText();
	const UTF32* end = begin + firstLine->getTextLength2() - 1;	// UITextDocumentLine 自体の Delim は含まない
	int firstNLPos, nlLen;
	if (StringTraits::indexOfNewLineSequence(begin, end, &firstNLPos, &nlLen))
	{
		// 今の Line を捨てる
		m_lines.removeAt(lineInsertIndex);




		//begin += firstNLPos + nlLen;

		// 後続の改行で分割し、新しい UITextDocumentLine を作っていく
		{
			const UTF32* lineBegin = begin;
			while (lineBegin < end)
			{
				int nlPos, nlLen;
				if (StringTraits::indexOfNewLineSequence(lineBegin, end, &nlPos, &nlLen))
				{
					auto line = newObject<UITextDocumentLine>(lineBegin, nlPos, UITextDocumentLine::NlCode);	// TODO: cache
					m_lines.insert(lineInsertIndex, line);

					lineInsertIndex++;
					lineBegin += nlPos + nlLen;
				}
				else
				{
					break;
				}
			}

			//if (lineBegin != end)
			{
				auto line = newObject<UITextDocumentLine>(lineBegin, end - lineBegin, firstLine->getLineDelimiterCode());	// TODO: cache
				m_lines.insert(lineInsertIndex, line);
				lineInsertIndex++;
			}
		}

		m_revision++;
	}
}

//------------------------------------------------------------------------------
int UITextDocument::findLineIndexFromOffset(int offset, int* outOffsetFromLineHead)
{
	int prevLineOffset = 0;
	int lineOffset = 0;
	for (int i = 0; i < m_lines.getCount(); i++)
	{
		lineOffset += m_lines[i]->getLength();
		if (offset < lineOffset)
		{
			if (outOffsetFromLineHead != nullptr)
			{
				*outOffsetFromLineHead = offset - prevLineOffset;
			}
			return i;
		}
		prevLineOffset = lineOffset;
	}
	return -1;
}

//------------------------------------------------------------------------------
UITextDocumentLine* UITextDocument::findLineFromOffset(int offset)
{
	int index = findLineIndexFromOffset(offset);
	if (index < 0) return nullptr;
	return m_lines[index];
}





//==============================================================================
// UITextVisualLine
//==============================================================================

//------------------------------------------------------------------------------
UITextVisualLine::UITextVisualLine()
	: m_textLength(0)
{
}

//------------------------------------------------------------------------------
UITextVisualLine::~UITextVisualLine()
{
}

//------------------------------------------------------------------------------
void UITextVisualLine::initialize()
{
}

//------------------------------------------------------------------------------
void UITextVisualLine::addGlyphRun(GlyphRun* run)
{
	m_glyphRuns.add(run);
	m_textLength += run->getTextLength();

	// ※横書き
	{
		m_renderRect.width += static_cast<float>(run->getRenderSize().width);
		m_renderRect.height = std::max(m_renderRect.height, static_cast<float>(run->getRenderSize().height));
	}
}

//------------------------------------------------------------------------------
void UITextVisualLine::render(DrawingContext* g)
{
	for (auto& run : m_glyphRuns)
	{
		g->drawGlyphRun(PointF(), run);// TODO:
	}
}

//------------------------------------------------------------------------------
Rect UITextVisualLine::getGlyphAreaGlobalRect(int column)
{
	Rect rect(0, m_renderRect.y, 0, m_renderRect.height);
	int count = 0;
	for (auto& run : m_glyphRuns)
	{
		if (column < (count + run->getTextLength()))
		{
			int localIndex = column - count;
			auto& items = run->requestLayoutItems();
			rect.x = items[localIndex].Location.BitmapTopLeftPosition.x;
			rect.width = items[localIndex].Location.BitmapSize.width;
			return rect;
		}
		count += run->getTextLength();
	}
	return rect;
}

//------------------------------------------------------------------------------
bool UITextVisualLine::testHitFromGlobalPoint(const PointF& pt, UITextVisualPosition* outPos)
{
	if (m_renderRect.contains(pt))
	{
		PointF localPt(pt.x - m_renderRect.x, pt.y - m_renderRect.y);
		int glyphIndex = 0;

		PointF flow(0, 0);
		for (auto& run : m_glyphRuns)
		{
			for (auto& item : run->requestLayoutItems())
			{
				Rect rc(flow, static_cast<float>(item.Location.BitmapSize.width), static_cast<float>(item.Location.BitmapSize.height));
				if (rc.contains(localPt))
				{
					outPos->line = m_visualLineIndex;
					outPos->column = glyphIndex;
					return true;
				}

				flow.x += item.Location.BitmapSize.width;
				glyphIndex++;
			}
		}
	}
	return false;
}

//==============================================================================
// UITextVisualLineBlock
//==============================================================================

//------------------------------------------------------------------------------
UITextVisualLineBlock::UITextVisualLineBlock()
	: m_revision(0)
{
}

//------------------------------------------------------------------------------
UITextVisualLineBlock::~UITextVisualLineBlock()
{
}

//------------------------------------------------------------------------------
void UITextVisualLineBlock::initialize(UITextDocumentLine* documentLine)
{
	m_documentLine = documentLine;
}

//------------------------------------------------------------------------------
bool UITextVisualLineBlock::isModified() const
{
	return m_documentLine->getRevision() != m_revision;
}

//------------------------------------------------------------------------------
void UITextVisualLineBlock::buildVisualLines(Font* font, int startDocumentTextOffset, int visualLineIndex, const PointF& renderPt)
{
	m_visualLines.clear();
	m_renderRect = Rect(renderPt, 0, 0);
	//m_renderTop = renderTop;
	//m_renderSize = Size::Zero;

	auto run = newObject<GlyphRun>();	// TODO: cache
	run->setFont(font->resolveRawFont());
	run->setText(m_documentLine->getText(), m_documentLine->getTextLength2());

	auto line = newObject<UITextVisualLine>();	// TODO: cache
	line->setStartDocumentTextOffset(startDocumentTextOffset);
	line->addGlyphRun(run);
	line->setVisualLineIndex(visualLineIndex);
	line->setRenderPt(renderPt);
	m_visualLines.add(line);

	// TODO: いまは物理行は1つしか作らないので
	m_renderRect = line->getRenderRect();


	// TODO: 折り返しの調整やハイライトはここで。
}

//------------------------------------------------------------------------------
void UITextVisualLineBlock::render(DrawingContext* g)
{
	for (auto& line : m_visualLines)
	{
		line->render(g);
	}
}


//==============================================================================
// UITextAreaCaret
//==============================================================================

//------------------------------------------------------------------------------
UITextAreaCaret::UITextAreaCaret()
{
}

//------------------------------------------------------------------------------
UITextAreaCaret::~UITextAreaCaret()
{
}

//------------------------------------------------------------------------------
void UITextAreaCaret::initialize()
{
	m_position.line = 0;
	m_position.column = 0;
}


//==============================================================================
// UITextArea
//==============================================================================

//------------------------------------------------------------------------------
UITextArea::UITextArea()
	: m_revision(0)
{
}

//------------------------------------------------------------------------------
UITextArea::~UITextArea()
{
}

//------------------------------------------------------------------------------
void UITextArea::initialize()
{
	m_document = newObject<UITextDocument>();
	m_caret = newObject<UITextAreaCaret>();
}

//------------------------------------------------------------------------------
Size UITextArea::measure(const Size& availableSize, Font* font)
{
	// 物理行リストの更新が必要？
	if (m_revision != m_document->getRevision())
	{
		// この状態で revision に差があるものは一度削除する。
		// 実際には削除されておらず変更されただけかもしれないが、簡略化のために一度削除する。
		for (int i = m_visualLineBlocks.getCount() - 1; i >= 0; i--)
		{
			if (m_visualLineBlocks[i]->isModified())
			{
				m_visualLineBlocks.removeAt(i);
			}
		}

		auto& documentLines = m_document->GetLines();
		int iDoc = 0;
		int iVisual = 0;
		for (; iDoc < documentLines.getCount() && iVisual < m_visualLineBlocks.getCount(); )
		{
			if (documentLines[iDoc] == m_visualLineBlocks[iVisual]->getDocumentLine())
			{
				iDoc++;
				iVisual++;
			}
			else
			{
				m_visualLineBlocks.insert(iVisual, newObject<UITextVisualLineBlock>(documentLines[iDoc]));
				iDoc++;
			}
		}
		for (; iDoc < documentLines.getCount(); iDoc++)
		{
			m_visualLineBlocks.add(newObject<UITextVisualLineBlock>(documentLines[iDoc]));
		}

		m_revision = m_document->getRevision();
	}



	
	{
		int documentTextOffset = 0;
		int visualLineIndex = 0;
		PointF renderPt;
		for (auto& block : m_visualLineBlocks)
		{
			block->buildVisualLines(font, documentTextOffset, visualLineIndex, renderPt);

			documentTextOffset += block->getDocumentLine()->getLength();
			visualLineIndex += block->getVisualLinesCount();
			renderPt.y += block->getRenderRect().height;
		}
	}


	return availableSize;
}

//------------------------------------------------------------------------------
Size UITextArea::Arrange(const Size& finalSize)
{
	return finalSize;
}

//------------------------------------------------------------------------------
void UITextArea::render(DrawingContext* g)
{
	for (auto& block : m_visualLineBlocks)
	{
		block->render(g);
	}


	// TODO: 毎回検索するのも時間かかりそう・・・
	Rect caretRect = getGlyphGlobalRectFromVisualPosition(m_caret->getVisualPosition());
	caretRect.width = 1;

	g->drawRectangle(caretRect);
}

//------------------------------------------------------------------------------
UITextVisualLine* UITextArea::findVisualLine(int lineIndex)
{
	int lineOffset = 0;
	for (int i = 0; i < m_visualLineBlocks.getCount(); i++)
	{
		int linesCount = m_visualLineBlocks[i]->getVisualLinesCount();
		if (lineIndex < (lineOffset + linesCount))
		{
			int localIndex = lineIndex - lineOffset;
			return m_visualLineBlocks[i]->getVisualLine(localIndex);
		}
		lineOffset += linesCount;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
Rect UITextArea::getGlyphGlobalRectFromVisualPosition(const UITextVisualPosition& pos)
{
	auto* visualLine = findVisualLine(pos.line);
	return visualLine->getGlyphAreaGlobalRect(pos.column);
}

//------------------------------------------------------------------------------
void UITextArea::getVisualPositionFromRenderPosition(const PointF& pt, UITextVisualPosition* outPos)
{
	outPos->line = 0;
	outPos->column = 0;

	for (auto& block : m_visualLineBlocks)
	{
		for (auto& line : block->getVisualLines())
		{
			if (line->testHitFromGlobalPoint(pt, outPos))
			{
				return;
			}
		}
	}
}

//------------------------------------------------------------------------------
int UITextArea::getDocumentTextOffset(const UITextVisualPosition& pos)
{
	auto* line = findVisualLine(pos.line);
	return line->getStartDocumentTextOffset() + pos.column;
}


//==============================================================================
// UITextBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITextBox, UITextElement)

//------------------------------------------------------------------------------
UITextBoxPtr UITextBox::create()
{
	auto ptr = Ref<UITextBox>::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UITextBox::UITextBox()
{
}

//------------------------------------------------------------------------------
UITextBox::~UITextBox()
{
}

//------------------------------------------------------------------------------
void UITextBox::initialize()
{
	UITextElement::initialize();
	setFocusable(true);
	//setHAlignment(HAlignment::Center);
	//setVAlignment(VAlignment::Center);

	m_textArea = newObject<UITextArea>();

	//m_document = newObject<tr::Document>();
	//m_documentView = newObject<tr::DocumentView>(m_document);



	// Test
	//auto d = newObject<UITextDocument>();
	//d->Replace(0, 0, _T("b"));
	//d->Replace(0, 0, _T("a"));
	//d->Replace(2, 0, _T("c"));
	//d->Replace(2, 0, _T("\n"));	// ab\nc
	//d->Replace(0, 4, _T("\n"));	// \n
}

//------------------------------------------------------------------------------
void UITextBox::setText(const StringRef& text)
{
	m_textArea->getDocument()->replace(0, 0, text);	// TODO:
}

//------------------------------------------------------------------------------
void UITextBox::onMouseDown(UIMouseEventArgs* e)
{
	UITextVisualPosition pos;
	m_textArea->getVisualPositionFromRenderPosition(e->getPosition(this), &pos);
	m_textArea->getCaret()->setVisualPosition(pos);

	UITextElement::onMouseDown(e);
}

//------------------------------------------------------------------------------
void UITextBox::onKeyDown(UIKeyEventArgs* e)
{
	

	UITextElement::onKeyDown(e);
}

//------------------------------------------------------------------------------
void UITextBox::onTextInput(UIKeyEventArgs* e)
{
	TCHAR ch = e->getCharCode();
	m_textArea->getDocument()->replace(
		m_textArea->getDocumentTextOffset(m_textArea->getCaret()->getVisualPosition()),
		0, StringRef(ch, 1));
}

//------------------------------------------------------------------------------
Size UITextBox::measureOverride(const Size& availableSize)
{
	Size size = UITextElement::measureOverride(availableSize);

	if (m_font != nullptr)
	{
		//Size textSize = m_font->measureRenderSize(m_text);
		//size.width = std::max(size.width, textSize.width);
		//size.height = std::max(size.height, textSize.height);
	}

	m_textArea->measure(availableSize, getActiveFont());

	return size;
}

//------------------------------------------------------------------------------
Size UITextBox::arrangeOverride(const Size& finalSize)
{
	m_textArea->Arrange(finalSize);

	return UITextElement::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UITextBox::onRender(DrawingContext* g)
{
	UITextElement::onRender(g);

	g->setFont(getActiveFont());	// TODO:
	g->setBrush(Brush::Red);
	m_textArea->render(g);
	//g->setFont(getActiveFont());
	//g->setBrush(getForegroundInternal());
	//g->drawText_(m_text, PointF::Zero);
	//g->drawChar('g', PointF(0, 0));
}




























class UISimpleTextArea
	: public UITextElement
{
public:
	UITextAreaCaret* getCaret() const { return m_caret; }

	void replace(int offset, int length, const StringRef& text);

	Size measure(const Size& availableSize, Font* font, detail::UIManager* manager);

protected:
	virtual void onMouseDown(UIMouseEventArgs* e) override;
	virtual void onTextInput(UIKeyEventArgs* e) override;

	virtual Size measureOverride(const Size& availableSize) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(DrawingContext* g) override;

LN_CONSTRUCT_ACCESS:
	UISimpleTextArea();
	virtual ~UISimpleTextArea();
	void initialize();
	//UITextVisualLine* findVisualLine(int lineIndex);
	//Rect getGlyphGlobalRectFromVisualPosition(const UITextVisualPosition& pos);
	//void getVisualPositionFromRenderPosition(const PointF& pt, UITextVisualPosition* outPos);
	//int getDocumentTextOffset(const UITextVisualPosition& pos);

private:
	void UpdateCaretRectangle();

	GenericStringBuilderCore<UTF32>	m_rawText;
	Ref<GlyphRun>				m_glyphRun;
	Ref<UITextAreaCaret>			m_caret;
	Ref<Brush>					m_caretBrush;
	bool							m_invalidateGlyphRun;
};


//==============================================================================
// UISimpleTextArea
//==============================================================================
//------------------------------------------------------------------------------
UISimpleTextArea::UISimpleTextArea()
	: m_invalidateGlyphRun(true)
{
}

//------------------------------------------------------------------------------
UISimpleTextArea::~UISimpleTextArea()
{
}

//------------------------------------------------------------------------------
void UISimpleTextArea::initialize()
{
	UITextElement::initialize();
	setFocusable(true);
	m_glyphRun = newObject<GlyphRun>();
	m_caret = newObject<UITextAreaCaret>();
	m_caretBrush = Brush::Black;
}

//------------------------------------------------------------------------------
void UISimpleTextArea::replace(int offset, int length, const StringRef& text)
{
	// to UTF32
	const ByteBuffer& utf32Buf = getManager()->getGraphicsManager()->getFontManager()->getTCharToUTF32Converter()->convert(text.getBegin(), sizeof(TCHAR) * text.getLength());
	int len = utf32Buf.getSize() / sizeof(UTF32);


	//m_rawText.Replace(offset, length, text.GetBegin(), text.GetLength());
	m_rawText.replace(offset, length, (const UTF32*)utf32Buf.getConstData(), len);
	m_invalidateGlyphRun = true;

	UITextVisualPosition pos = { 0, offset + len };
	m_caret->setVisualPosition(pos);
}

//------------------------------------------------------------------------------
void UISimpleTextArea::onMouseDown(UIMouseEventArgs* e)
{
	//UITextVisualPosition pos;
	//m_textArea->getVisualPositionFromRenderPosition(e->GetPosition(this), &pos);
	//m_textArea->getCaret()->setVisualPosition(pos);

	UITextElement::onMouseDown(e);
}

//------------------------------------------------------------------------------
void UISimpleTextArea::onTextInput(UIKeyEventArgs* e)
{
	//m_textArea->getDocument()->Replace(
	//	m_textArea->getDocumentTextOffset(m_textArea->getCaret()->getVisualPosition()),
	//	0, StringRef(&e->charCode, 1));

	TCHAR ch = e->getCharCode();
	replace(m_caret->getVisualPosition().column, 0, StringRef(&ch, 1));
	UITextElement::onTextInput(e);
}

//------------------------------------------------------------------------------
Size UISimpleTextArea::measureOverride(const Size& availableSize)
{
	Size ds = UITextElement::measureOverride(availableSize);
	return Size::max(ds, measure(availableSize, getActiveFont(), getManager()));
}

//------------------------------------------------------------------------------
Size UISimpleTextArea::arrangeOverride(const Size& finalSize)
{
	return UITextElement::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UISimpleTextArea::onRender(DrawingContext* g)
{
	UITextElement::onRender(g);

	g->setFont(getActiveFont());	// TODO:
	g->setBrush(Brush::Red);

	g->drawGlyphRun(PointF(0, 0), m_glyphRun);

	g->setBrush(m_caretBrush);
	g->drawRectangle(m_caret->getRenderRectangle());
	//g->setFont(getActiveFont());
	//g->setBrush(getForegroundInternal());
	//g->drawText_(m_text, PointF::Zero);
	//g->drawChar('g', PointF(0, 0));
}

//------------------------------------------------------------------------------
Size UISimpleTextArea::measure(const Size& availableSize, Font* font, detail::UIManager* manager)
{
	m_glyphRun->setFont(font->resolveRawFont());
	if (m_invalidateGlyphRun)
	{

		//m_glyphRun->SetText((const UTF32*)utf32Buf.GetConstData(), len);
		m_glyphRun->setText(m_rawText.c_str(), m_rawText.getLength());
		m_invalidateGlyphRun = false;


		UpdateCaretRectangle();
	}

	return m_glyphRun->getRenderSize().toFloatSize();
}

//------------------------------------------------------------------------------
void UISimpleTextArea::UpdateCaretRectangle()
{
	Rect rect(0, 0, 1, m_glyphRun->getRenderSize().height);
	PointF pos;
	if (m_glyphRun->getDistanceFromCharacterHit(m_caret->getVisualPosition().column, &pos))
	{
		pos.x += 1;	// 少しだけ間を空けて見やすくする
		rect.setLocation(pos);
	}
	m_caret->setRenderRectangle(rect);
}


//==============================================================================
// UITextField
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITextField, UITextElement)

//------------------------------------------------------------------------------
Ref<UITextField> UITextField::create()
{
	return newObject<UITextField>();
}

//------------------------------------------------------------------------------
UITextField::UITextField()
{
}

//------------------------------------------------------------------------------
UITextField::~UITextField()
{
}

//------------------------------------------------------------------------------
void UITextField::initialize()
{
	UIControl::initialize();
	setFocusable(true);
	m_textArea = newObject<UISimpleTextArea>();

	m_textArea->setBackground(Brush::Blue);
	addVisualChild(m_textArea);
}

//------------------------------------------------------------------------------
void UITextField::setText(const StringRef& text)
{
	m_textArea->replace(0, 0, text);
}

//------------------------------------------------------------------------------
Size UITextField::measureOverride(const Size& availableSize)
{
	m_textArea->measureLayout(availableSize);
	return UIControl::measureOverride(availableSize);
}

//------------------------------------------------------------------------------
Size UITextField::arrangeOverride(const Size& finalSize)
{
	m_textArea->arrangeLayout(Rect(0, 0, finalSize));
	return UIControl::arrangeOverride(finalSize);
}

LN_NAMESPACE_END

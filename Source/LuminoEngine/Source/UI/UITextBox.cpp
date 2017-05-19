
#include "Internal.h"
#include <Lumino/Graphics/DrawingContext.h>
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

	int GetLength() const { return m_text.GetLength()/* + 1*/; }	//// LineDelimiter 分の +1
	bool IsEmpty() const { return m_text.IsEmpty(); }

LN_CONSTRUCT_ACCESS:
	UITextDocumentLine();
	virtual ~UITextDocumentLine();
	void Initialize();
	void Initialize(const UTF32* text, int len, UTF32 lineDelim);

LN_INTERNAL_ACCESS:
	void RemoveInternal(int offset, int length);
	void InsertText(int offset, const UTF32* text, int len);
	//void AppendText(const UTF32* text, int len);
	void SetLineDelimiter(LineDelimiter delim) { m_lineDelimiter = delim; }
	UTF32 GetLineDelimiterCode() const { return m_text.c_str()[m_text.GetLength() - 1]; }
	const UTF32* GetText() const { return m_text.c_str(); }
	int GetTextLength2() const { return m_text.GetLength() - 1; }
	void Clear();
	int GetRevision() const { return m_revision; }

private:
	GenericStringBuilderCore<UTF32>	m_text;				// \n を含む
	LineDelimiter					m_lineDelimiter;
	int								m_revision;
};

class UITextDocument
	: public Object//tr::DocumentContentElement
{
public:
	void Replace(int offset, int length, const StringRef& text);

LN_CONSTRUCT_ACCESS:
	UITextDocument();
	virtual ~UITextDocument();
	void Initialize();

LN_INTERNAL_ACCESS:
	int GetRevision() const { return m_revision; }
	const List<RefPtr<UITextDocumentLine>>& GetLines() const { return m_lines; }

private:
	void ReplaceInternal(int offset, int length, const UTF32* text, int len);
	void RemoveInternal(int offset, int length);
	void InsertInternal(int offset, const UTF32* text, int len);
	int FindLineIndexFromOffset(int offset, int* outOffsetFromLineHead = nullptr);	// 0~
	UITextDocumentLine* FindLineFromOffset(int offset);

	detail::UIManager*					m_manager;
	List<RefPtr<UITextDocumentLine>>	m_lines;		// 必ず1つ入っている。
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
	const Size& GetRenderSize() const { return m_renderRect.GetSize(); }
	const Rect& GetRenderRect() const { return m_renderRect; }

LN_CONSTRUCT_ACCESS:
	UITextVisualLine();
	virtual ~UITextVisualLine();
	void Initialize();

LN_INTERNAL_ACCESS:
	void SetStartDocumentTextOffset(int offset) { m_startDocumentTextOffset = offset; }
	int GetStartDocumentTextOffset() const { return  m_startDocumentTextOffset; }
	void AddGlyphRun(GlyphRun* run);
	void SetVisualLineIndex(int index) { m_visualLineIndex = index; }
	void SetRenderPt(const PointF& pt) { m_renderRect.SetLocation(pt); }
	void Render(DrawingContext* g);
	Rect GetGlyphAreaGlobalRect(int column);	// 高さ=行高さ
	bool TestHitFromGlobalPoint(const PointF& pt, UITextVisualPosition* outPos);
	int GetTextLength() const { return m_textLength; }

private:
	List<RefPtr<GlyphRun>>	m_glyphRuns;
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
	bool IsModified() const;
	UITextDocumentLine* GetDocumentLine() const { return m_documentLine; }

	int GetVisualLinesCount() const { return m_visualLines.GetCount(); }
	UITextVisualLine* GetVisualLine(int index) const { return m_visualLines[index]; }

	//float GetRenderTop() const { return m_renderTop; }
	//float GetRenderHeight() const { return m_renderSize.width; }
	const Rect& GetRenderRect() const { return m_renderRect; }

	void BuildVisualLines(Font* font, int startDocumentTextOffset, int visualLineIndex, const PointF& renderPt);

LN_CONSTRUCT_ACCESS:
	UITextVisualLineBlock();
	virtual ~UITextVisualLineBlock();
	void Initialize(UITextDocumentLine* documentLine);

LN_INTERNAL_ACCESS:
	void Render(DrawingContext* g);
	const List<RefPtr<UITextVisualLine>>& GetVisualLines() const { return m_visualLines; }

private:
	RefPtr<UITextDocumentLine>		m_documentLine;
	int								m_revision;
	List<RefPtr<UITextVisualLine>>	m_visualLines;
	Rect							m_renderRect;	// TextArea 座標系
	//float							m_renderTop;
	//Size							m_renderSize;
};




class UITextAreaCaret
	: public Object
{
public:
	void SetVisualPosition(const UITextVisualPosition& pos) { m_position = pos; }

LN_CONSTRUCT_ACCESS:
	UITextAreaCaret();
	virtual ~UITextAreaCaret();
	void Initialize();

LN_INTERNAL_ACCESS:
	const UITextVisualPosition& GetVisualPosition() const { return m_position; }
	void SetRenderRectangle(const Rect& rect) { m_renderRectangle = rect; }
	const Rect& GetRenderRectangle() const { return m_renderRectangle; }

private:
	UITextVisualPosition	m_position;
	Rect					m_renderRectangle;
};


class UITextArea
	: public Object
{

public:
	UITextDocument* GetDocument() const { return m_document; }
	UITextAreaCaret* GetCaret() const { return m_caret; }

	Size Measure(const Size& availableSize, Font* font);
	Size Arrange(const Size& finalSize);
	void Render(DrawingContext* g);

LN_CONSTRUCT_ACCESS:
	UITextArea();
	virtual ~UITextArea();
	void Initialize();

LN_INTERNAL_ACCESS:
	UITextVisualLine* FindVisualLine(int lineIndex);
	Rect GetGlyphGlobalRectFromVisualPosition(const UITextVisualPosition& pos);
	void GetVisualPositionFromRenderPosition(const PointF& pt, UITextVisualPosition* outPos);
	int GetDocumentTextOffset(const UITextVisualPosition& pos);

private:
	RefPtr<UITextDocument>				m_document;
	List<RefPtr<UITextVisualLineBlock>>	m_visualLineBlocks;
	int									m_revision;
	RefPtr<UITextAreaCaret>				m_caret;
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
void UITextDocumentLine::Initialize()
{
	Clear();
}

//------------------------------------------------------------------------------
void UITextDocumentLine::Initialize(const UTF32* text, int len, UTF32 lineDelim)
{
	m_text.Append(text, len);
	m_text.Append(lineDelim);
	m_revision++;
}

//------------------------------------------------------------------------------
void UITextDocumentLine::RemoveInternal(int offset, int length)
{
	// adjust range
	int rangeBegin = offset;
	int rangeEnd = offset + length;
	if (rangeBegin < 0) rangeBegin = 0;
	if (rangeEnd > m_text.GetLength()) rangeEnd = m_text.GetLength();

	UTF32 empty[] = { 0 };
	m_text.Replace(rangeBegin, rangeEnd - rangeBegin, empty, 0);	// TODO: 普通に Remove 関数とか作ったほうがパフォーマンスよさそう
	m_revision++;
}

//------------------------------------------------------------------------------
void UITextDocumentLine::InsertText(int offset, const UTF32* text, int len)
{
	m_text.Replace(offset, 0, text, len);	// TODO: 普通に Insert 関数とか作ったほうがパフォーマンスよさそう
	m_revision++;
}

//------------------------------------------------------------------------------
//void UITextDocumentLine::AppendText(const UTF32* text, int len)
//{
//	m_text.Append(text, len);
//}

//------------------------------------------------------------------------------
void UITextDocumentLine::Clear()
{
	m_text.Clear();
	m_text.Append(EofCode);
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
void UITextDocument::Initialize()
{
	m_manager = detail::EngineDomain::GetUIManager();

	m_lines.Add(NewObject<UITextDocumentLine>());	// 空line。EOF。	// TODO: cache
	m_revision++;
}

//------------------------------------------------------------------------------
void UITextDocument::Replace(int offset, int length, const StringRef& text)
{
	// UTF32 へ変換
	const ByteBuffer& utf32Buf = m_manager->GetGraphicsManager()->GetFontManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), sizeof(TCHAR) * text.GetLength());
	int len = utf32Buf.GetSize() / sizeof(UTF32);
	ReplaceInternal(offset, length, (const UTF32*)utf32Buf.GetConstData(), len);
}

//------------------------------------------------------------------------------
void UITextDocument::ReplaceInternal(int offset, int length, const UTF32* text, int len)
{
	RemoveInternal(offset, length);
	InsertInternal(offset, text, len);
}

//------------------------------------------------------------------------------
void UITextDocument::RemoveInternal(int offset, int length)
{
	int offsetFromLineHead;
	int index = FindLineIndexFromOffset(offset, &offsetFromLineHead);
	int beginOffset = offset;
	int endOffset = offset + length;
	while (beginOffset < endOffset)
	{
		int removeLength = endOffset - beginOffset;
		removeLength = std::min(removeLength, m_lines[index]->GetLength());

		m_lines[index]->RemoveInternal(offsetFromLineHead, removeLength);

		beginOffset += removeLength;
		offsetFromLineHead = 0;
		index++;
	}

	// GC empty lines
	for (int i = m_lines.GetCount() - 1; i >= 0; i--)
	{
		if (m_lines[i]->IsEmpty())
		{
			if (m_lines.GetCount() == 1)
			{
				// 最後の1つなら Remove はせず、空行にする
				m_lines[i]->Clear();
			}
			else
			{
				m_lines.RemoveAt(i);
			}
			m_revision++;
		}
	}
}

//------------------------------------------------------------------------------
void UITextDocument::InsertInternal(int offset, const UTF32* text, int len)
{
	//UITextDocumentLine* line = FindLineFromOffset(offset);
	//if (line != nullptr)
	//{
	//	m_lines.Add(NewObject<UITextDocumentLine>());	// TODO: cache
	//	line = m_lines.GetLast();
	//}

	int offsetFromLineHead = 0;
	int lineInsertIndex = FindLineIndexFromOffset(offset, &offsetFromLineHead);
	//if (lineInsertIndex < 0)
	//{
	//	m_lines.Add(NewObject<UITextDocumentLine>());	// 空line。EOF。	// TODO: cache
	//	lineInsertIndex = 0;
	//}
	RefPtr<UITextDocumentLine> firstLine = m_lines[lineInsertIndex];

	// まず全部 Insert する
	firstLine->InsertText(offsetFromLineHead, text, len);
	






	// 改行が含まれているか？
	const UTF32* begin = firstLine->GetText();
	const UTF32* end = begin + firstLine->GetTextLength2() - 1;	// UITextDocumentLine 自体の Delim は含まない
	int firstNLPos, nlLen;
	if (StringTraits::IndexOfNewLineSequence(begin, end, &firstNLPos, &nlLen))
	{
		// 今の Line を捨てる
		m_lines.RemoveAt(lineInsertIndex);




		//begin += firstNLPos + nlLen;

		// 後続の改行で分割し、新しい UITextDocumentLine を作っていく
		{
			const UTF32* lineBegin = begin;
			while (lineBegin < end)
			{
				int nlPos, nlLen;
				if (StringTraits::IndexOfNewLineSequence(lineBegin, end, &nlPos, &nlLen))
				{
					auto line = NewObject<UITextDocumentLine>(lineBegin, nlPos, UITextDocumentLine::NlCode);	// TODO: cache
					m_lines.Insert(lineInsertIndex, line);

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
				auto line = NewObject<UITextDocumentLine>(lineBegin, end - lineBegin, firstLine->GetLineDelimiterCode());	// TODO: cache
				m_lines.Insert(lineInsertIndex, line);
				lineInsertIndex++;
			}
		}

		m_revision++;
	}
}

//------------------------------------------------------------------------------
int UITextDocument::FindLineIndexFromOffset(int offset, int* outOffsetFromLineHead)
{
	int prevLineOffset = 0;
	int lineOffset = 0;
	for (int i = 0; i < m_lines.GetCount(); i++)
	{
		lineOffset += m_lines[i]->GetLength();
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
UITextDocumentLine* UITextDocument::FindLineFromOffset(int offset)
{
	int index = FindLineIndexFromOffset(offset);
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
void UITextVisualLine::Initialize()
{
}

//------------------------------------------------------------------------------
void UITextVisualLine::AddGlyphRun(GlyphRun* run)
{
	m_glyphRuns.Add(run);
	m_textLength += run->GetTextLength();

	// ※横書き
	{
		m_renderRect.width += static_cast<float>(run->GetRenderSize().width);
		m_renderRect.height = std::max(m_renderRect.height, static_cast<float>(run->GetRenderSize().height));
	}
}

//------------------------------------------------------------------------------
void UITextVisualLine::Render(DrawingContext* g)
{
	for (auto& run : m_glyphRuns)
	{
		g->DrawGlyphRun(PointF(), run);// TODO:
	}
}

//------------------------------------------------------------------------------
Rect UITextVisualLine::GetGlyphAreaGlobalRect(int column)
{
	Rect rect(0, m_renderRect.y, 0, m_renderRect.height);
	int count = 0;
	for (auto& run : m_glyphRuns)
	{
		if (column < (count + run->GetTextLength()))
		{
			int localIndex = column - count;
			auto& items = run->RequestLayoutItems();
			rect.x = items[localIndex].Location.BitmapTopLeftPosition.x;
			rect.width = items[localIndex].Location.BitmapSize.width;
			return rect;
		}
		count += run->GetTextLength();
	}
	return rect;
}

//------------------------------------------------------------------------------
bool UITextVisualLine::TestHitFromGlobalPoint(const PointF& pt, UITextVisualPosition* outPos)
{
	if (m_renderRect.Contains(pt))
	{
		PointF localPt(pt.x - m_renderRect.x, pt.y - m_renderRect.y);
		int glyphIndex = 0;

		PointF flow(0, 0);
		for (auto& run : m_glyphRuns)
		{
			for (auto& item : run->RequestLayoutItems())
			{
				Rect rc(flow, static_cast<float>(item.Location.BitmapSize.width), static_cast<float>(item.Location.BitmapSize.height));
				if (rc.Contains(localPt))
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
void UITextVisualLineBlock::Initialize(UITextDocumentLine* documentLine)
{
	m_documentLine = documentLine;
}

//------------------------------------------------------------------------------
bool UITextVisualLineBlock::IsModified() const
{
	return m_documentLine->GetRevision() != m_revision;
}

//------------------------------------------------------------------------------
void UITextVisualLineBlock::BuildVisualLines(Font* font, int startDocumentTextOffset, int visualLineIndex, const PointF& renderPt)
{
	m_visualLines.Clear();
	m_renderRect = Rect(renderPt, 0, 0);
	//m_renderTop = renderTop;
	//m_renderSize = Size::Zero;

	auto run = NewObject<GlyphRun>();	// TODO: cache
	run->SetFont(font->ResolveRawFont());
	run->SetText(m_documentLine->GetText(), m_documentLine->GetTextLength2());

	auto line = NewObject<UITextVisualLine>();	// TODO: cache
	line->SetStartDocumentTextOffset(startDocumentTextOffset);
	line->AddGlyphRun(run);
	line->SetVisualLineIndex(visualLineIndex);
	line->SetRenderPt(renderPt);
	m_visualLines.Add(line);

	// TODO: いまは物理行は1つしか作らないので
	m_renderRect = line->GetRenderRect();


	// TODO: 折り返しの調整やハイライトはここで。
}

//------------------------------------------------------------------------------
void UITextVisualLineBlock::Render(DrawingContext* g)
{
	for (auto& line : m_visualLines)
	{
		line->Render(g);
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
void UITextAreaCaret::Initialize()
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
void UITextArea::Initialize()
{
	m_document = NewObject<UITextDocument>();
	m_caret = NewObject<UITextAreaCaret>();
}

//------------------------------------------------------------------------------
Size UITextArea::Measure(const Size& availableSize, Font* font)
{
	// 物理行リストの更新が必要？
	if (m_revision != m_document->GetRevision())
	{
		// この状態で revision に差があるものは一度削除する。
		// 実際には削除されておらず変更されただけかもしれないが、簡略化のために一度削除する。
		for (int i = m_visualLineBlocks.GetCount() - 1; i >= 0; i--)
		{
			if (m_visualLineBlocks[i]->IsModified())
			{
				m_visualLineBlocks.RemoveAt(i);
			}
		}

		auto& documentLines = m_document->GetLines();
		int iDoc = 0;
		int iVisual = 0;
		for (; iDoc < documentLines.GetCount() && iVisual < m_visualLineBlocks.GetCount(); )
		{
			if (documentLines[iDoc] == m_visualLineBlocks[iVisual]->GetDocumentLine())
			{
				iDoc++;
				iVisual++;
			}
			else
			{
				m_visualLineBlocks.Insert(iVisual, NewObject<UITextVisualLineBlock>(documentLines[iDoc]));
				iDoc++;
			}
		}
		for (; iDoc < documentLines.GetCount(); iDoc++)
		{
			m_visualLineBlocks.Add(NewObject<UITextVisualLineBlock>(documentLines[iDoc]));
		}

		m_revision = m_document->GetRevision();
	}



	
	{
		int documentTextOffset = 0;
		int visualLineIndex = 0;
		PointF renderPt;
		for (auto& block : m_visualLineBlocks)
		{
			block->BuildVisualLines(font, documentTextOffset, visualLineIndex, renderPt);

			documentTextOffset += block->GetDocumentLine()->GetLength();
			visualLineIndex += block->GetVisualLinesCount();
			renderPt.y += block->GetRenderRect().height;
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
void UITextArea::Render(DrawingContext* g)
{
	for (auto& block : m_visualLineBlocks)
	{
		block->Render(g);
	}


	// TODO: 毎回検索するのも時間かかりそう・・・
	Rect caretRect = GetGlyphGlobalRectFromVisualPosition(m_caret->GetVisualPosition());
	caretRect.width = 1;

	g->DrawRectangle(caretRect);
}

//------------------------------------------------------------------------------
UITextVisualLine* UITextArea::FindVisualLine(int lineIndex)
{
	int lineOffset = 0;
	for (int i = 0; i < m_visualLineBlocks.GetCount(); i++)
	{
		int linesCount = m_visualLineBlocks[i]->GetVisualLinesCount();
		if (lineIndex < (lineOffset + linesCount))
		{
			int localIndex = lineIndex - lineOffset;
			return m_visualLineBlocks[i]->GetVisualLine(localIndex);
		}
		lineOffset += linesCount;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
Rect UITextArea::GetGlyphGlobalRectFromVisualPosition(const UITextVisualPosition& pos)
{
	auto* visualLine = FindVisualLine(pos.line);
	return visualLine->GetGlyphAreaGlobalRect(pos.column);
}

//------------------------------------------------------------------------------
void UITextArea::GetVisualPositionFromRenderPosition(const PointF& pt, UITextVisualPosition* outPos)
{
	outPos->line = 0;
	outPos->column = 0;

	for (auto& block : m_visualLineBlocks)
	{
		for (auto& line : block->GetVisualLines())
		{
			if (line->TestHitFromGlobalPoint(pt, outPos))
			{
				return;
			}
		}
	}
}

//------------------------------------------------------------------------------
int UITextArea::GetDocumentTextOffset(const UITextVisualPosition& pos)
{
	auto* line = FindVisualLine(pos.line);
	return line->GetStartDocumentTextOffset() + pos.column;
}


//==============================================================================
// UITextBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITextBox, UITextElement)

//------------------------------------------------------------------------------
UITextBoxPtr UITextBox::Create()
{
	auto ptr = RefPtr<UITextBox>::MakeRef();
	ptr->Initialize();
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
void UITextBox::Initialize()
{
	UITextElement::Initialize();
	SetFocusable(true);
	//SetHAlignment(HAlignment::Center);
	//SetVAlignment(VAlignment::Center);

	m_textArea = NewObject<UITextArea>();

	//m_document = NewObject<tr::Document>();
	//m_documentView = NewObject<tr::DocumentView>(m_document);



	// Test
	//auto d = NewObject<UITextDocument>();
	//d->Replace(0, 0, _T("b"));
	//d->Replace(0, 0, _T("a"));
	//d->Replace(2, 0, _T("c"));
	//d->Replace(2, 0, _T("\n"));	// ab\nc
	//d->Replace(0, 4, _T("\n"));	// \n
}

//------------------------------------------------------------------------------
void UITextBox::SetText(const StringRef& text)
{
	m_textArea->GetDocument()->Replace(0, 0, text);	// TODO:
}

//------------------------------------------------------------------------------
void UITextBox::OnMouseDown(UIMouseEventArgs* e)
{
	UITextVisualPosition pos;
	m_textArea->GetVisualPositionFromRenderPosition(e->GetPosition(this), &pos);
	m_textArea->GetCaret()->SetVisualPosition(pos);

	UITextElement::OnMouseDown(e);
}

//------------------------------------------------------------------------------
void UITextBox::OnKeyDown(UIKeyEventArgs* e)
{
	

	UITextElement::OnKeyDown(e);
}

//------------------------------------------------------------------------------
void UITextBox::OnTextInput(UIKeyEventArgs* e)
{
	TCHAR ch = e->GetCharCode();
	m_textArea->GetDocument()->Replace(
		m_textArea->GetDocumentTextOffset(m_textArea->GetCaret()->GetVisualPosition()),
		0, StringRef(ch, 1));
}

//------------------------------------------------------------------------------
Size UITextBox::MeasureOverride(const Size& availableSize)
{
	Size size = UITextElement::MeasureOverride(availableSize);

	if (m_font != nullptr)
	{
		//Size textSize = m_font->MeasureRenderSize(m_text);
		//size.width = std::max(size.width, textSize.width);
		//size.height = std::max(size.height, textSize.height);
	}

	m_textArea->Measure(availableSize, GetActiveFont());

	return size;
}

//------------------------------------------------------------------------------
Size UITextBox::ArrangeOverride(const Size& finalSize)
{
	m_textArea->Arrange(finalSize);

	return UITextElement::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UITextBox::OnRender(DrawingContext* g)
{
	UITextElement::OnRender(g);

	g->SetFont(GetActiveFont());	// TODO:
	g->SetBrush(Brush::Red);
	m_textArea->Render(g);
	//g->SetFont(GetActiveFont());
	//g->SetBrush(GetForegroundInternal());
	//g->DrawText_(m_text, PointF::Zero);
	//g->DrawChar('g', PointF(0, 0));
}




























class UISimpleTextArea
	: public UITextElement
{
public:
	UITextAreaCaret* GetCaret() const { return m_caret; }

	void Replace(int offset, int length, const StringRef& text);

	Size Measure(const Size& availableSize, Font* font, detail::UIManager* manager);

protected:
	virtual void OnMouseDown(UIMouseEventArgs* e) override;
	virtual void OnTextInput(UIKeyEventArgs* e) override;

	virtual Size MeasureOverride(const Size& availableSize) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRender(DrawingContext* g) override;

LN_CONSTRUCT_ACCESS:
	UISimpleTextArea();
	virtual ~UISimpleTextArea();
	void Initialize();
	//UITextVisualLine* FindVisualLine(int lineIndex);
	//Rect GetGlyphGlobalRectFromVisualPosition(const UITextVisualPosition& pos);
	//void GetVisualPositionFromRenderPosition(const PointF& pt, UITextVisualPosition* outPos);
	//int GetDocumentTextOffset(const UITextVisualPosition& pos);

private:
	void UpdateCaretRectangle();

	GenericStringBuilderCore<UTF32>	m_rawText;
	RefPtr<GlyphRun>				m_glyphRun;
	RefPtr<UITextAreaCaret>			m_caret;
	RefPtr<Brush>					m_caretBrush;
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
void UISimpleTextArea::Initialize()
{
	UITextElement::Initialize();
	SetFocusable(true);
	m_glyphRun = NewObject<GlyphRun>();
	m_caret = NewObject<UITextAreaCaret>();
	m_caretBrush = Brush::Black;
}

//------------------------------------------------------------------------------
void UISimpleTextArea::Replace(int offset, int length, const StringRef& text)
{
	// to UTF32
	const ByteBuffer& utf32Buf = GetManager()->GetGraphicsManager()->GetFontManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), sizeof(TCHAR) * text.GetLength());
	int len = utf32Buf.GetSize() / sizeof(UTF32);


	//m_rawText.Replace(offset, length, text.GetBegin(), text.GetLength());
	m_rawText.Replace(offset, length, (const UTF32*)utf32Buf.GetConstData(), len);
	m_invalidateGlyphRun = true;

	UITextVisualPosition pos = { 0, offset + len };
	m_caret->SetVisualPosition(pos);
}

//------------------------------------------------------------------------------
void UISimpleTextArea::OnMouseDown(UIMouseEventArgs* e)
{
	//UITextVisualPosition pos;
	//m_textArea->GetVisualPositionFromRenderPosition(e->GetPosition(this), &pos);
	//m_textArea->GetCaret()->SetVisualPosition(pos);

	UITextElement::OnMouseDown(e);
}

//------------------------------------------------------------------------------
void UISimpleTextArea::OnTextInput(UIKeyEventArgs* e)
{
	//m_textArea->GetDocument()->Replace(
	//	m_textArea->GetDocumentTextOffset(m_textArea->GetCaret()->GetVisualPosition()),
	//	0, StringRef(&e->charCode, 1));

	TCHAR ch = e->GetCharCode();
	Replace(m_caret->GetVisualPosition().column, 0, StringRef(&ch, 1));
	UITextElement::OnTextInput(e);
}

//------------------------------------------------------------------------------
Size UISimpleTextArea::MeasureOverride(const Size& availableSize)
{
	Size ds = UITextElement::MeasureOverride(availableSize);
	return Size::Max(ds, Measure(availableSize, GetActiveFont(), GetManager()));
}

//------------------------------------------------------------------------------
Size UISimpleTextArea::ArrangeOverride(const Size& finalSize)
{
	return UITextElement::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UISimpleTextArea::OnRender(DrawingContext* g)
{
	UITextElement::OnRender(g);

	g->SetFont(GetActiveFont());	// TODO:
	g->SetBrush(Brush::Red);

	g->DrawGlyphRun(PointF(0, 0), m_glyphRun);

	g->SetBrush(m_caretBrush);
	g->DrawRectangle(m_caret->GetRenderRectangle());
	//g->SetFont(GetActiveFont());
	//g->SetBrush(GetForegroundInternal());
	//g->DrawText_(m_text, PointF::Zero);
	//g->DrawChar('g', PointF(0, 0));
}

//------------------------------------------------------------------------------
Size UISimpleTextArea::Measure(const Size& availableSize, Font* font, detail::UIManager* manager)
{
	m_glyphRun->SetFont(font->ResolveRawFont());
	if (m_invalidateGlyphRun)
	{

		//m_glyphRun->SetText((const UTF32*)utf32Buf.GetConstData(), len);
		m_glyphRun->SetText(m_rawText.c_str(), m_rawText.GetLength());
		m_invalidateGlyphRun = false;


		UpdateCaretRectangle();
	}

	return m_glyphRun->GetRenderSize().ToFloatSize();
}

//------------------------------------------------------------------------------
void UISimpleTextArea::UpdateCaretRectangle()
{
	Rect rect(0, 0, 1, m_glyphRun->GetRenderSize().height);
	PointF pos;
	if (m_glyphRun->GetDistanceFromCharacterHit(m_caret->GetVisualPosition().column, &pos))
	{
		pos.x += 1;	// 少しだけ間を空けて見やすくする
		rect.SetLocation(pos);
	}
	m_caret->SetRenderRectangle(rect);
}


//==============================================================================
// UITextField
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITextField, UITextElement)

//------------------------------------------------------------------------------
RefPtr<UITextField> UITextField::Create()
{
	return NewObject<UITextField>();
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
void UITextField::Initialize()
{
	UIControl::Initialize();
	SetFocusable(true);
	m_textArea = NewObject<UISimpleTextArea>();

	m_textArea->SetBackground(Brush::Blue);
	AddVisualChild(m_textArea);
}

//------------------------------------------------------------------------------
void UITextField::SetText(const StringRef& text)
{
	m_textArea->Replace(0, 0, text);
}

//------------------------------------------------------------------------------
Size UITextField::MeasureOverride(const Size& availableSize)
{
	m_textArea->MeasureLayout(availableSize);
	return UIControl::MeasureOverride(availableSize);
}

//------------------------------------------------------------------------------
Size UITextField::ArrangeOverride(const Size& finalSize)
{
	m_textArea->ArrangeLayout(Rect(0, 0, finalSize));
	return UIControl::ArrangeOverride(finalSize);
}

LN_NAMESPACE_END

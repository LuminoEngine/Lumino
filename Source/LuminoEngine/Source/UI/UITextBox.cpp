
#include "Internal.h"
#include <Lumino/Graphics/Rendering.h>
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


//class UITextVisualElement
//	: public Object
//{
//
//};


// 物理行
class UITextVisualLine
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
	UITextVisualLine();
	virtual ~UITextVisualLine();
	void Initialize();

LN_INTERNAL_ACCESS:
	void AddGlyphRun(GlyphRun* run) { m_glyphRuns.Add(run); }
	void Render(DrawList* g);

private:
	List<RefPtr<GlyphRun>>	m_glyphRuns;
};

// 物理行のまとまり
class UITextVisualLineBlock
	: public Object
{
public:
	bool IsModified() const;
	UITextDocumentLine* GetDocumentLine() const { return m_documentLine; }

	void BuildVisualLines(Font* font);

LN_CONSTRUCT_ACCESS:
	UITextVisualLineBlock();
	virtual ~UITextVisualLineBlock();
	void Initialize(UITextDocumentLine* documentLine);

LN_INTERNAL_ACCESS:
	void Render(DrawList* g);

private:
	RefPtr<UITextDocumentLine>		m_documentLine;
	int								m_revision;
	List<RefPtr<UITextVisualLine>>	m_visualLines;
};

struct UITextVisualPosition
{
	int	line;
	int column;
};



class UITextAreaCaret
	: public Object
{
LN_CONSTRUCT_ACCESS:
	UITextAreaCaret();
	virtual ~UITextAreaCaret();
	void Initialize();
};


class UITextArea
	: public Object
{

public:
	UITextDocument* GetDocument() const { return m_document; }

	Size Measure(const Size& availableSize, Font* font);
	Size Arrange(const Size& finalSize);
	void Render(DrawList* g);

LN_CONSTRUCT_ACCESS:
	UITextArea();
	virtual ~UITextArea();
	void Initialize();

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
void UITextVisualLine::Render(DrawList* g)
{
	for (auto& run : m_glyphRuns)
	{
		g->DrawGlyphRun(PointF(), run);// TODO:
	}
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
void UITextVisualLineBlock::BuildVisualLines(Font* font)
{
	m_visualLines.Clear();

	auto run = NewObject<GlyphRun>();	// TODO: cache
	run->SetFont(font->ResolveRawFont());
	run->SetText(m_documentLine->GetText(), m_documentLine->GetTextLength2());

	auto line = NewObject<UITextVisualLine>();	// TODO: cache
	line->AddGlyphRun(run);
	m_visualLines.Add(line);

	// TODO: 折り返しの調整やハイライトはここで。
}

//------------------------------------------------------------------------------
void UITextVisualLineBlock::Render(DrawList* g)
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
		for (auto& block : m_visualLineBlocks)
		{
			block->BuildVisualLines(font);
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
void UITextArea::Render(DrawList* g)
{
	for (auto& block : m_visualLineBlocks)
	{
		block->Render(g);
	}
}


//==============================================================================
// UITextBox
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITextBox, UITextElement)

//------------------------------------------------------------------------------
UITextBoxPtr UITextBox::Create()
{
	auto ptr = RefPtr<UITextBox>::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
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
void UITextBox::Initialize(detail::UIManager* manager)
{
	UITextElement::Initialize(manager);
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
bool UITextBox::IsFocusable() const
{
	return true;
}

//------------------------------------------------------------------------------
void UITextBox::OnTextInput(UIKeyEventArgs* e)
{
	m_textArea->GetDocument()->Replace(0, 0, StringRef(&e->charCode, 1));
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
void UITextBox::OnRender(DrawList* g)
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

LN_NAMESPACE_END

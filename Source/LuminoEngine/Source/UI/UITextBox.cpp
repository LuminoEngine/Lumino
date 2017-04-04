
#include "Internal.h"
#include <Lumino/Graphics/Rendering.h>
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
	int GetTextLength() const { return m_text.GetLength(); }
	void Clear();

private:
	GenericStringBuilderCore<UTF32>	m_text;				// \n を含む
	LineDelimiter					m_lineDelimiter;
};

class UITextDocument
	: public tr::DocumentContentElement
{
public:
	void Replace(int offset, int length, const StringRef& text);

LN_CONSTRUCT_ACCESS:
	UITextDocument();
	virtual ~UITextDocument();
	void Initialize();

private:
	void ReplaceInternal(int offset, int length, const UTF32* text, int len);
	void RemoveInternal(int offset, int length);
	void InsertInternal(int offset, const UTF32* text, int len);
	int FindLineIndexFromOffset(int offset, int* outOffsetFromLineHead = nullptr);	// 0~
	UITextDocumentLine* FindLineFromOffset(int offset);

	detail::UIManager*					m_manager;
	List<RefPtr<UITextDocumentLine>>	m_lines;		// 必ず1つ入っている。
};


//class UITextVisualElement
//	: public Object
//{
//
//};

class UITextVisualLine
	: public Object
{

private:
	int		m_revision;
};



class UITextArea	// TODO: AvalonDock 的には "View"
	: public Object
{

private:
	RefPtr<UITextDocument>			m_document;
	List<RefPtr<UITextVisualLine>>	m_visualLines;
	int								m_revision;
};




//==============================================================================
// UITextDocumentLine
//==============================================================================

//------------------------------------------------------------------------------
UITextDocumentLine::UITextDocumentLine()
	: m_text()
	, m_lineDelimiter(LineDelimiter::EndOfFile)
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
}

//------------------------------------------------------------------------------
void UITextDocumentLine::InsertText(int offset, const UTF32* text, int len)
{
	m_text.Replace(offset, 0, text, len);	// TODO: 普通に Insert 関数とか作ったほうがパフォーマンスよさそう
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
}

//==============================================================================
// UITextDocument
//==============================================================================

//------------------------------------------------------------------------------
UITextDocument::UITextDocument()
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
	const UTF32* end = begin + firstLine->GetTextLength() - 1;	// UITextDocumentLine 自体の Delim は含まない
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

		// offset 位置の行に、まだ \n を含むバッファがあるので縮める
		//firstLine->RemoveInternal(firstNLPos, firstLine->GetTextLength() - firstNLPos);
		//firstLine->AppendText(&nlCode, 1);

		// 最後に追加された行の LineDelimiter を決める
		//m_lines[lineInsertIndex - 1]->SetLineDelimiter(firstLineDelim);
	}

#if 0
	const UTF32* lineBegin = text;
	const UTF32* end = text + len;
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
	if (lineBegin != end)
	{
		auto line = NewObject<UITextDocumentLine>(lineBegin, end - lineBegin, firstLine->GetLineDelimiterCode());	// TODO: cache
		m_lines.Insert(lineInsertIndex, line);
	}
#endif

#if 0
	const UTF32* begin = firstLine->GetText();
	const UTF32* end = begin + firstLine->GetTextLength();
	int firstNLPos, nlLen;
	if (StringTraits::IndexOfNewLineSequence(begin, end, &firstNLPos, &nlLen))
	{
		//UITextDocumentLine::LineDelimiter firstLineDelim = firstLine->GetLineDelimiter();

		//firstLine->SetLineDelimiter(UITextDocumentLine::LineDelimiter::LF);	// TODO: とりあえず固定
		begin += firstNLPos + nlLen;
		//lineInsertIndex++;

		// 後続の改行で分割し、新しい UITextDocumentLine を作っていく
		{
			const UTF32* lineBegin = begin;
			while (lineBegin < end)
			{
				int nlPos, nlLen;
				if (StringTraits::IndexOfNewLineSequence(lineBegin, end, &nlPos, &nlLen))
				{
					auto line = NewObject<UITextDocumentLine>(lineBegin, nlPos, UITextDocumentLine::NlCode);	// TODO: cache
					//line->SetLineDelimiter(UITextDocumentLine::LineDelimiter::LF);	// TODO: とりあえず固定
					m_lines.Insert(lineInsertIndex, line);

					lineInsertIndex++;
					lineBegin += nlPos + nlLen;
				}
				else
				{
					break;
				}
			}
			if (lineBegin != end)
			{
				auto line = NewObject<UITextDocumentLine>(lineBegin, end - lineBegin);	// TODO: cache
				//line->AppendText(&nlCode, 1);
				line->SetLineDelimiter(UITextDocumentLine::LineDelimiter::LF);	// TODO: とりあえず固定
				m_lines.Insert(lineInsertIndex, line);
				lineInsertIndex++;
			}
		}

		// offset 位置の行に、まだ \n を含むバッファがあるので縮める
		//firstLine->RemoveInternal(firstNLPos, firstLine->GetTextLength() - firstNLPos);
		//firstLine->AppendText(&nlCode, 1);

		// 最後に追加された行の LineDelimiter を決める
		//m_lines[lineInsertIndex - 1]->SetLineDelimiter(firstLineDelim);
	}
#endif



	



#if 0

	const UTF32* lineBegin = text;
	const UTF32* end = text + len;
	while (lineBegin < end)
	{
		int nlPos, nlLen;
		if (StringTraits::IndexOfNewLineSequence(lineBegin, end, &nlPos, &nlLen))
		{
			if (targetLine == nullptr)
			{
				auto line = NewObject<UITextDocumentLine>();
				m_lines.Insert(lineInsertIndex, targetLine);
				targetLine = line;
			}

			targetLine->InsertText(offsetFromLineHead, lineBegin, nlPos);
			targetLine->SetLineDelimiter(UITextDocumentLine::LineDelimiter::LF);	// TODO: とりあえず固定

			offsetFromLineHead = 0;
			targetLine = nullptr;
			lineInsertIndex++;
			lineBegin += nlPos + nlLen;
		}
		else
		{
			break;
		}
	}
	if (lineBegin != end)
	{
		if (targetLine == nullptr)
		{
			auto line = NewObject<UITextDocumentLine>();
			m_lines.Insert(lineInsertIndex, targetLine);
			targetLine = line;
		}

		targetLine->InsertText(offsetFromLineHead, lineBegin, end - lineBegin);
		targetLine->SetLineDelimiter(UITextDocumentLine::LineDelimiter::LF);	// TODO: とりあえず固定
	}
#endif
#if 0
	// Split

	int lineInsertIndex = 0;
	if (!m_lines.IsEmpty())
	{
		int offsetFromLineHead;
		lineInsertIndex = FindLineIndexFromOffset(offset, &offsetFromLineHead);
		if (offsetFromLineHead == 0)
		{
			// offset は行頭を指しているので、lineInsertIndex はそのままで良い
		}
		else
		{

		}
	}



	// Append

	const UTF32 newLineCode = '\n';
	const UTF32* lineBegin = text;
	const UTF32* end = text + len;
	while (lineBegin < end)
	{
		int nlPos, nlLen;
		if (StringTraits::IndexOfNewLineSequence(lineBegin, end, &nlPos, &nlLen))
		{
			// 改行を付けて UITextDocumentLine を作る
			auto line = NewObject<UITextDocumentLine>(lineBegin, nlPos);	// TODO: cache
			line->AppendText(&newLineCode, 1);
			m_lines.Insert(lineInsertIndex, line);
			lineInsertIndex++;
			lineBegin += nlPos + nlLen;
		}
		else
		{
			break;
		}
	}
	if (lineBegin != end)
	{
		// 改行を付けないで UITextDocumentLine を作る
		auto line = NewObject<UITextDocumentLine>(lineBegin, end - lineBegin);	// TODO: cache
		m_lines.Insert(lineInsertIndex, line);
	}


	// Merge
#endif
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

	m_document = NewObject<tr::Document>();
	m_documentView = NewObject<tr::DocumentView>(m_document);



	// Test
	auto d = NewObject<UITextDocument>();
	d->Replace(0, 0, _T("b"));
	d->Replace(0, 0, _T("a"));
	d->Replace(2, 0, _T("c"));
	d->Replace(2, 0, _T("\n"));	// ab\nc
	d->Replace(0, 4, _T("\n"));	// \n
}

//------------------------------------------------------------------------------
void UITextBox::SetText(const StringRef& text)
{
	m_document->SetText(text);
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

	m_documentView->MeasureLayout(availableSize);

	return size;
}

//------------------------------------------------------------------------------
Size UITextBox::ArrangeOverride(const Size& finalSize)
{
	m_documentView->ArrangeLayout(RectF(0, 0, finalSize));

	return UITextElement::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UITextBox::OnRender(DrawList* g)
{
	UITextElement::OnRender(g);

	g->SetFont(GetActiveFont());	// TODO:
	m_documentView->Render(g);
	//g->SetFont(GetActiveFont());
	//g->SetBrush(GetForegroundInternal());
	//g->DrawText_(m_text, PointF::Zero);
	//g->DrawChar('g', PointF(0, 0));
}

LN_NAMESPACE_END

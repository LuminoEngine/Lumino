/*
	[2015/8/25]
		・実際に画面に表示する Inline は Visual なんたらにしておく。
		　これはいうなれば論理要素に対する物理要素。
		 　オブジェクト化しておくことで、アニメーションもやりやすい。

		・AvalonEdit では、物理ピクセル→論理行の変換は Rendering.HeightTree の役目。
		Rendering.HeightTree は、この変換というか検索を高速に行う赤黒木。

		・Visual の作成は Measure で。

		・改行は1文字。\r\nも1文字。
*/
#include "../Internal.h"
#include <Lumino/Platform/Clipboard.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/GUI/TextBox.h>
#include <Lumino/Documents/DocumentsRenderer.h>
#include <Lumino/Documents/Run.h>
#include "GUIManagerImpl.h"
#include "../Animation/AnimationCurve.h"
#include "../Graphics/FontGlyphTextureCache.h"
#include "../Graphics/GraphicsHelper.h"
#include "GUIHelper.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

class TextBox::Selection
{
public:
	int	Start;		///< text.GetLength() は有効値。Select(text.GetLength(), 0) でコンテンツの末尾にキャレットを移動する
	int Length;

public:
	Selection()
		: Start(0)
		, Length(0)
	{}
};


// 本格的なテキストエディタであれば装飾のため、さらに単語単位に区切ったりする。
// ここの TextBox は文字ごとに装飾とかは考えないシンプルなものなので、まずは行単位。
// なお、改行文字は含まない。
class TextBox::LineSegment
	: public RefObject
{
public:
	LineSegment(Document* document, const TCHAR* str, int len);

	void UpdateGlyphRuns();


	int GetLength() const
	{
		return m_utf32Text.GetLength();
	}

	int GetLinePixelWidth() const
	{
		return m_glyphRun->GetRenderSize().Width;
	}

	int GetLinePixelHeight() const
	{
		return m_glyphRun->GetRenderSize().Height;
	}

	int GetPixelPosFromLineHead(int contentPos)
	{
		if (contentPos == 0) { return 0; }
		int c = contentPos - 1;
		UpdateGlyphRuns();
		TextLayoutResult* r = Helper::GetGlyphData(m_glyphRun);
		FontGlyphLocation* loc = &r->Items[c].Location;
		return loc->OuterTopLeftPosition.X + loc->BitmapSize.Width;
	}

	// localPt.Y はこの LineSegment の中にあることが前提。
	// 戻り値はこの LineSegment 内の Index
	int GetContentIndexFromPixel(const Point& localPt)
	{
		//const Array<Graphics::TextLayoutResultItem>& items = m_glyphRun->GetImtes();
		const Array<TextLayoutResultItem>& items = Helper::GetGlyphData(m_glyphRun)->Items;

		for (int i = 0; i < items.GetCount(); ++i)
		{
			int left;
			if (i == 0) { // 行頭の文字は左端から
				left = 0; 
			}
			else {
				left = items[i - 1].Location.OuterTopLeftPosition.X + items[i - 1].Location.BitmapSize.Width / 2;
			}

			int right = items[i].Location.OuterTopLeftPosition.X + items[i].Location.BitmapSize.Width / 2;
			
			

			if (left <= localPt.X && localPt.X <= right) {
				return i;
			}
		}
		return items.GetCount();
	}

	void Replace(int start, int length, const uint32_t* text, int len)
	{
		m_utf32Text.Replace(start, length, text, len);
		m_textModified = true;
		UpdateGlyphRuns();
	}


public:
	Document*		m_document;
	GenericStringBuilderCore<UTF32>	m_utf32Text;
	//int				m_realCharCount;

	RefPtr<GlyphRun>		m_glyphRun;	// 本来なら View と分けるべき。ただ、今回はシンプル重視で。
	bool			m_textModified;
};

class TextBox::Document
{
public:
	struct SelectionRenderingInfo
	{
		int Length;
		int StartLineNumber;
		int EndLineNumber;
		RectF StartLineRect;	// Start のピクセル位置から行末まで
		RectF EndLineRect;		// 行頭から End まで
	};

public:
	//int GetLength() const {  }

	Document()
		: m_selectionRenderingInfoModified(true)
	{
		m_TCharToUTF32Converter.SetDestinationEncoding(Encoding::GetUTF32Encoding());
		m_TCharToUTF32Converter.SetSourceEncoding(Encoding::GetTCharEncoding());
		m_UTF32ToTCharConverter.SetDestinationEncoding(Encoding::GetTCharEncoding());
		m_UTF32ToTCharConverter.SetSourceEncoding(Encoding::GetUTF32Encoding());
	}

	void SetFontGlyphTextureCache(Internal::FontGlyphTextureCache* cache)
	{
		m_glyphTextureCache = cache;
		for (auto& seg : m_lineSegments) {
			seg->UpdateGlyphRuns();
		}
	}

	Internal::FontGlyphTextureCache* GetFontGlyphTextureCache()
	{
		return m_glyphTextureCache;
	}

	void Replace(int start, int length, const String& text)
	{
		// TODO:とりあえず初回前提
		const TCHAR* pos = text.GetCStr();
		const TCHAR* end = pos + text.GetLength();
		int nlIndex = 0, nlCount = 0;
		while (StringTraits::IndexOfNewLineSequence(pos, end, &nlIndex, &nlCount))
		{
			RefPtr<TextBox::LineSegment> line(LN_NEW LineSegment(this, pos, nlIndex));
			m_lineSegments.Add(line);
			pos += (nlIndex + nlCount);	// 改行文字の次の文字を指す
		}
		if (pos != end)
		{
			RefPtr<TextBox::LineSegment> line(LN_NEW LineSegment(this, pos, end - pos));
			m_lineSegments.Add(line);
		}
		m_selectionRenderingInfoModified = true;
	}

	// UTF32文字数を返す
	int Replace(const Selection* sel, const String& text)
	{
		const ByteBuffer& utf32 = m_TCharToUTF32Converter.Convert(text.GetCStr(), text.GetByteCount());
		int len = utf32.GetSize() / sizeof(UTF32);
		Replace(sel, (const UTF32*)utf32.GetConstData(), len);
		m_selectionRenderingInfoModified = true;
		return len;
	}

	void Replace(const Selection* sel, const UTF32* text, int len)
	{
		int beginLineNumber, beginPosFromLineHead;
		int endLineNumber, endPosFromLineHead;
		GetLineNumber(sel->Start, &beginLineNumber, &beginPosFromLineHead);
		GetLineNumber(sel->Start + sel->Length, &endLineNumber, &endPosFromLineHead);
		if (beginLineNumber == endLineNumber) {
			m_lineSegments[beginLineNumber]->Replace(beginPosFromLineHead, sel->Length, text, len);
		}
		else
		{
			LN_THROW(0, NotImplementedException);
		}
		m_selectionRenderingInfoModified = true;
	}

	// 指定したコンテンツ位置 (文字位置) が何行目かを返す
	void GetLineNumber(int contentPos, int* lineNumber, int* lenFromLineHead, int* yLocation = NULL)
	{
		int len = 0;
		int y = 0;
		for (int line = 0; line < m_lineSegments.GetCount(); ++line)
		{
			int head = len;
			len += m_lineSegments[line]->GetLength();
			if (contentPos <= len) {
				*lenFromLineHead = contentPos - head;
				if (yLocation != NULL) { *yLocation = y; }
				*lineNumber = line;
				return;
			}
			y += m_lineSegments[line]->GetLinePixelHeight();
		}
		LN_THROW(0, InvalidOperationException);
	}



	int GetContentIndexFromPixel(const Point& pt)
	{
		int len = 0;
		int y = 0;
		for (int line = 0; line < m_lineSegments.GetCount(); ++line)
		{
			int h = m_lineSegments[line]->GetLinePixelHeight();
			len += m_lineSegments[line]->GetLength();
			if (pt.Y <= pt.Y && pt.Y <= y + h) {
				return m_lineSegments[line]->GetContentIndexFromPixel(pt);
			}
			y += h;
		}
		return len;
	}

	// calet を描画するべき左上の座標
	// TODO: キャレットは RenderSelection で書くようにすればこれいらない
	Point GetCaretLocation(int contentPos, int* curLineHeight)
	{
		int lineNumber, lenFromLineHead, yLocation;
		GetLineNumber(contentPos, &lineNumber, &lenFromLineHead, &yLocation);
		*curLineHeight = m_lineSegments[lineNumber]->GetLinePixelHeight();
		return Point(
			m_lineSegments[lineNumber]->GetPixelPosFromLineHead(lenFromLineHead),
			yLocation);
	}

	const Array< RefPtr<TextBox::LineSegment> >& GetLineSegments() const { return m_lineSegments; }

	void ModifySelection()
	{
		m_selectionRenderingInfoModified = true;
	}

	void UpdateSelectionRenderingInfo(const Selection* sel)
	{
		int len = 0;
		int y = 0;
		int line = 0;
		m_selectionRenderingInfo.StartLineNumber = -1;
		m_selectionRenderingInfo.EndLineNumber = -1;
		m_selectionRenderingInfo.Length = sel->Length;

		// start 位置の情報を取得する
		for (; line < m_lineSegments.GetCount(); ++line)
		{
			int head = len;
			if (sel->Start <= len + m_lineSegments[line]->GetLength())
			{
				int lenFromLineHead = sel->Start - head;
				int x = m_lineSegments[line]->GetPixelPosFromLineHead(lenFromLineHead);
				m_selectionRenderingInfo.StartLineRect.Set(x, y, m_lineSegments[line]->GetLinePixelWidth(), m_lineSegments[line]->GetLinePixelHeight());
				m_selectionRenderingInfo.StartLineNumber = line;
				break;
			}
			len += m_lineSegments[line]->GetLength();
			y += m_lineSegments[line]->GetLinePixelHeight();
		}

		// end 位置の情報を取得する
		for (; line < m_lineSegments.GetCount(); ++line)	// 開始時、line は start のある行とおなじ行を指している
		{
			int head = len;
			len += m_lineSegments[line]->GetLength();
			if (sel->Start + sel->Length <= len + m_lineSegments[line]->GetLength())
			{
				int lenFromLineHead = (sel->Start + sel->Length) - head;
				int x = m_lineSegments[line]->GetPixelPosFromLineHead(lenFromLineHead);
				m_selectionRenderingInfo.EndLineRect.Set(x, y, m_lineSegments[line]->GetLinePixelWidth(), m_lineSegments[line]->GetLinePixelHeight());
				m_selectionRenderingInfo.EndLineNumber = line;
				break;
			}
			len += m_lineSegments[line]->GetLength();
			y += m_lineSegments[line]->GetLinePixelHeight();
		}

		// ちゃんと取得できていれば行番号があるはず
		LN_CHECK_STATE(m_selectionRenderingInfo.StartLineNumber != -1);
		LN_CHECK_STATE(m_selectionRenderingInfo.EndLineNumber != -1);
	}

	// 選択範囲を描画する。ブラシはあらかじめセットしておくこと。
	void RenderSelection(Painter* painter, const Selection* sel)
	{
		if (m_selectionRenderingInfoModified)
		{
			UpdateSelectionRenderingInfo(sel);
			m_selectionRenderingInfoModified = false;
		}

		// 選択範囲の背景
		if (m_selectionRenderingInfo.Length > 0)
		{
			if (m_selectionRenderingInfo.StartLineNumber == m_selectionRenderingInfo.EndLineNumber)
			{
				RectF rc(
					m_selectionRenderingInfo.StartLineRect.X,
					m_selectionRenderingInfo.StartLineRect.Y,
					m_selectionRenderingInfo.EndLineRect.X - m_selectionRenderingInfo.StartLineRect.X,
					m_selectionRenderingInfo.StartLineRect.Height);
				painter->SetBrush(ColorBrush::Blue);
				painter->DrawRectangle(rc);
			}
			else {
				LN_THROW(0, NotImplementedException);
			}
		}
	}

	void Render(Painter* painter)
	{
		Point pt(0, 0);
		for (auto& seg : m_lineSegments)
		{
			seg->UpdateGlyphRuns();
			painter->DrawGlyphRun(pt, seg->m_glyphRun);
			pt.Y += seg->m_glyphRun->GetRenderSize().Height;
		}
	}


public:
	Array< RefPtr<TextBox::LineSegment> >	m_lineSegments;
	RefPtr<Internal::FontGlyphTextureCache>	m_glyphTextureCache;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;
	SelectionRenderingInfo	m_selectionRenderingInfo;
	bool					m_selectionRenderingInfoModified;
};



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::LineSegment::LineSegment(Document* document, const TCHAR* str, int len)
	: m_document(document)
	, m_textModified(true)
{
	m_glyphRun.Attach(LN_NEW GlyphRun());
	m_utf32Text.Clear();
	m_utf32Text.Append(m_document->m_TCharToUTF32Converter.Convert(str, len * sizeof(TCHAR)));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::LineSegment::UpdateGlyphRuns()
{
	if (m_textModified)
	{
		Helper::AttachGlyphTextureCache(m_glyphRun, m_document->GetFontGlyphTextureCache());
		m_glyphRun->Update(m_utf32Text.GetCStr(), m_utf32Text.GetLength());
		m_textModified = false;
	}
}


//=============================================================================
// TextBox
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBox, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBox);

LN_PROPERTY_IMPLEMENT_GETTER_SETTER(TextBox, String, TextProperty, "Text", get_Text, set_Text, PropertyMetadata(String::GetEmpty()));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox* TextBox::Create(GUIManagerImpl* manager)
{
	RefPtr<TextBox> obj(LN_NEW TextBox(manager));
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::TextBox(GUIManagerImpl* manager)
	: Control(manager)
	, m_document(NULL)
	, m_selection(NULL)
	, m_caretAnimation(NULL)
	, m_isDragging(false)
{
	m_document = LN_NEW Document();
	m_selection = LN_NEW Selection();
	m_caretAnimation = LN_NEW Animation::FloatAnimationCurve();
	m_caretAnimation->SetWrapMode(Animation::WrapMode_Loop);

	Animation::FloatKeyFrame key;
	key.Mode = Animation::InterpolationMode_None;
	key.FrameTime = 0.0;
	key.Value = 1.0f;
	m_caretAnimation->AddKeyFrame(key);
	key.FrameTime = 0.5;
	key.Value = 0.0f;
	m_caretAnimation->AddKeyFrame(key);
	key.FrameTime = 1.0;
	key.Value = 1.0f;
	m_caretAnimation->AddKeyFrame(key);

	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(TextBox, KeyEventArgs, UIElement::KeyDownEvent, Handler_KeyDown);
	LN_REGISTER_ROUTED_EVENT_HANDLER(TextBox, KeyEventArgs, UIElement::TextInputEvent, Handler_TextInput);
	LN_REGISTER_ROUTED_EVENT_HANDLER(TextBox, MouseEventArgs, UIElement::MouseDownEvent, Handler_MouseDown);
	LN_REGISTER_ROUTED_EVENT_HANDLER(TextBox, MouseEventArgs, UIElement::MouseUpEvent, Handler_MouseUp);
	LN_REGISTER_ROUTED_EVENT_HANDLER(TextBox, MouseEventArgs, UIElement::MouseMoveEvent, Handler_MouseMove);

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBox::~TextBox()
{
	LN_SAFE_RELEASE(m_caretAnimation);
	LN_SAFE_DELETE(m_selection);
	LN_SAFE_DELETE(m_document);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Select(int start, int length)
{
	m_selection->Start = start;
	m_selection->Length = length;
	m_document->ModifySelection();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Paste()
{
	String text = Platform::Clipboard::GetText(m_manager->GetNativeWindow());
	int len = m_document->Replace(m_selection, text);
	Select(m_selection->Start += len, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBox::MeasureOverride(const SizeF& constraint)
{
	if (GUIHelper::UIElement_GetInvalidateFlags(this).TestFlag(InvalidateFlags::Font))
	{
		GraphicsManager::FontData fontData;
		fontData.Family = GetFontFamily();
		fontData.Size = GetFontSize();
		fontData.IsBold = IsFontBold();
		fontData.IsItalic = IsFontItalic();
		fontData.IsAntiAlias = IsFontAntiAlias();
		RefPtr<Internal::FontGlyphTextureCache> cache = m_manager->GetGraphicsManager()->LookupGlyphTextureCache(fontData);
		m_document->SetFontGlyphTextureCache(cache);
	}
	return Control::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::OnRender(Painter* painter)
{
	Control::OnRender(painter);
	m_document->RenderSelection(painter, m_selection);

	
	Brush* brush = GetForeground();
	if (brush == NULL) { brush = ColorBrush::Black; }
	painter->SetBrush(brush);
	m_document->Render(painter);

	m_caretAnimation->SetTime(m_manager->GetTime());

	// 選択領域


	// キャレット
	int caretHeight;
	Point caretPos = m_document->GetCaretLocation(m_selection->Start + m_selection->Length, &caretHeight);
	RectF caret((float)caretPos.X, (float)caretPos.Y, 1.0f, (float)caretHeight);
	painter->SetBrush(ColorBrush::Red);
	painter->SetOpacity(m_caretAnimation->GetValue());
	painter->DrawRectangle(caret);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::set_Text(const String& string)
{
	// TODO: プロパティ初期値セットされてない
	m_document->Replace(0, 0, string);	// TODO: GetLength()
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const String& TextBox::get_Text() const
{
	return String::GetEmpty();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Handler_KeyDown(KeyEventArgs* e)
{
	/* TODO:
		本来であれば、ショートカットは InputBinding を使って実現するべき。
		ただ、現時点でショートカットが必要なコントロールは TextBox だけ。
		Menu とかは直近では考えていない。
		それなので、ショートカットの検出もとりあえずここでやってしまう。
	*/
	if (e->KeyCode == Key_V && e->IsControl)
	{
		Paste();
		e->Handled = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Handler_TextInput(KeyEventArgs* e)
{
	UTF32 buf[1] = { e->Char };
	m_document->Replace(m_selection, buf, 1);
	m_selection->Start++;
	m_selection->Length = 0;
	m_document->ModifySelection();
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Handler_MouseDown(MouseEventArgs* e)
{
	Point pt(e->X, e->Y);
	m_dragStartContentIndex = m_document->GetContentIndexFromPixel(pt);
	Select(m_dragStartContentIndex, 0);

	if (!m_isDragging)
	{
		CaptureMouse();
		m_isDragging = true;
		e->Handled = true;
		return;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Handler_MouseUp(MouseEventArgs* e)
{
	if (m_isDragging)
	{
		m_isDragging = false;
		ReleaseMouseCapture();
		e->Handled = true;
		return;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBox::Handler_MouseMove(MouseEventArgs* e)
{
	if (m_isDragging)
	{
		Point pt(e->X, e->Y);
		int index = m_document->GetContentIndexFromPixel(pt);
		if (index < m_dragStartContentIndex) {
			Select(index, m_dragStartContentIndex - index);
		}
		else {
			Select(m_dragStartContentIndex, index - m_dragStartContentIndex);
		}

		e->Handled = true;
		return;
	}
}

LN_NAMESPACE_GUI_END
} // namespace Lumino


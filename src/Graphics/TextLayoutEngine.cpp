
#include "../Internal.h"
#include "TextLayoutEngine.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// TextLayoutEngine
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextLayoutEngine::TextLayoutEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextLayoutEngine::~TextLayoutEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextLayoutEngine::ResetSettings()
{
	m_textAlignment = TextAlignment::Left;
	m_textTrimming = TextTrimming::None;
	m_flowDirection = FlowDirection::LeftToRight;
	m_drawingArea = Rect::Zero;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextLayoutEngine::LayoutText(const UTF32* text, int length, TextLayoutResult* outResult/*, bool takeOverKerning*/)
{
	m_result = outResult;

	if (m_flowDirection == FlowDirection::LeftToRight)
	{
		LayoutTextHorizontal(text, length);
	}
	else if (m_flowDirection == FlowDirection::RightToLeft)
	{
		LN_THROW(0, NotImplementedException);
	}
	else if (m_flowDirection == FlowDirection::TopToBottom)
	{
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextLayoutEngine::LayoutTextHorizontal(const UTF32* text, int length)
{
	m_result->AreaSize = Size::Zero;
	m_result->Items.Clear();

	Rect rc = m_drawingArea;
	Size lineSize;
	int begin = 0;	// 1行の開始位置
	int i = 0;
	for (; i < length; ++i)
	{
		// LF
		if (text[i] == 0x0A)
		{
			LayoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
			rc.Y += lineSize.Height;
			rc.Height -= lineSize.Height;
			begin = i + 1;
			m_result->AreaSize.Width = std::max(m_result->AreaSize.Width, lineSize.Width);
			m_result->AreaSize.Height += lineSize.Height;
		}
		if (text[i] == 0x0D)
		{
			// CR + LF
			if (i < length - 1 && text[i + 1] == 0x0A)
			{
				LayoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
				rc.Y += lineSize.Height;
				rc.Height -= lineSize.Height;
				i++;
				begin = i + 1;
				m_result->AreaSize.Width = std::max(m_result->AreaSize.Width, lineSize.Width);
				m_result->AreaSize.Height += lineSize.Height;
			}
			// CR
			else
			{
				LayoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
				rc.Y += lineSize.Height;
				rc.Height -= lineSize.Height;
				begin = i;
				begin = i + 1;
				m_result->AreaSize.Width = std::max(m_result->AreaSize.Width, lineSize.Width);
				m_result->AreaSize.Height += lineSize.Height;
			}
		}
	}
	if (begin != i)
	{
		LayoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
		m_result->AreaSize.Width = std::max(m_result->AreaSize.Width, lineSize.Width);
		m_result->AreaSize.Height += lineSize.Height;
	}

	// TODO: アンチエイリアス有効だと、↑の方法では1px 足りないことがあった。
	// とりあえずここで強制設定している。
	m_result->AreaSize.Height = m_font->GetLineHeight();
}

//-----------------------------------------------------------------------------
// lineArea : 行を描画できる領域。Y は左上の Y 座標。Bottom は描画領域の下端。
//-----------------------------------------------------------------------------
void TextLayoutEngine::LayoutLineHorizontal(const UTF32* text, int length, const Rect& lineArea, Size* outLineSize)
{
	outLineSize->Set(0, 0);

	// 描く必要がない
	if (length == 0) { return; }

	// まずは左詰めで計測する
	FontGlyphLocation* prevInfo = NULL;
	int planeWidth = 0;
	for (int i = 0; i < length; ++i)
	{
		prevInfo = m_font->AdvanceKerning(text[i], prevInfo);
		planeWidth += prevInfo->BitmapSize.Width;
		outLineSize->Height = std::max(outLineSize->Height, prevInfo->BitmapSize.Height);

		TextLayoutResultItem item;
		item.Char = text[i];
		item.Location = *prevInfo;
		m_result->Items.Add(item);
	}

	// TODO: GUI から呼ばれるときは、UI要素のサイズを図る目的でも使用する。
	// TextAlignment::Left 固定でいいのか考えておく。
	outLineSize->Width = planeWidth;

	// 各グリフの配置を決める
	Rect drawArea = lineArea;
	switch (m_textAlignment)
	{
	case TextAlignment::Left:
	{
		break;
	}
	case TextAlignment::Center:
	{
		int offset = (lineArea.Width - planeWidth) / 2;
		for (TextLayoutResultItem & item : m_result->Items) {
			item.Location.BitmapTopLeftPosition.X += offset;
			item.Location.OutlineBitmapTopLeftPosition.X += offset;
			item.Location.OuterTopLeftPosition.X += offset;
		}
		break;
	}
	case TextAlignment::Right:
	{
		int offset = lineArea.GetRight() - planeWidth;
		for (TextLayoutResultItem & item : m_result->Items) {
			item.Location.BitmapTopLeftPosition.X += offset;
			item.Location.OutlineBitmapTopLeftPosition.X += offset;
			item.Location.OuterTopLeftPosition.X += offset;
		}
		break;
	}
	case TextAlignment::Justify:
		LN_THROW(0, NotImplementedException);
		break;
	}

	// ... トリミング
	switch (m_textTrimming)
	{
	case TextTrimming::None:
		break;
	case TextTrimming::CharacterEllipsis:
		LN_THROW(0, NotImplementedException);
		break;
	}
}

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino

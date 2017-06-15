
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "TextLayoutEngine.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// TextLayoutEngine
//==============================================================================

//------------------------------------------------------------------------------
TextLayoutEngine::TextLayoutEngine()
	: m_strokeSize(0)
	, m_textAlignment(TextAlignment::Left)
	, m_textTrimming(TextTrimming::None)
	, m_flowDirection(FlowDirection::LeftToRight)
{
}

//------------------------------------------------------------------------------
TextLayoutEngine::~TextLayoutEngine()
{
}

//------------------------------------------------------------------------------
void TextLayoutEngine::resetSettings()
{
	m_strokeSize = 0;
	m_textAlignment = TextAlignment::Left;
	m_textTrimming = TextTrimming::None;
	m_flowDirection = FlowDirection::LeftToRight;
	m_drawingArea = RectI::Zero;
}

//------------------------------------------------------------------------------
void TextLayoutEngine::layoutText(const UTF32* text, int length, LayoutTextOptions options, TextLayoutResult* outResult/*, bool takeOverKerning*/)
{
	m_result = outResult;
	m_layoutTextOptions = options;

	if (m_flowDirection == FlowDirection::LeftToRight)
	{
		layoutTextHorizontal(text, length);
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

//------------------------------------------------------------------------------
void TextLayoutEngine::layoutTextHorizontal(const UTF32* text, int length)
{
	m_result->AreaSize = SizeI::Zero;

	if (m_layoutTextOptions == LayoutTextOptions::All)
	{
		m_result->Items.clear();
	}

	RectI rc = m_drawingArea;
	SizeI lineSize;
	int begin = 0;	// 1行の開始位置
	int i = 0;
	for (; i < length; ++i)
	{
		// LF
		if (text[i] == 0x0A)
		{
			layoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
			rc.y += lineSize.height;
			rc.height -= lineSize.height;
			begin = i + 1;
			m_result->AreaSize.width = std::max(m_result->AreaSize.width, lineSize.width);
			m_result->AreaSize.height += lineSize.height;
		}
		if (text[i] == 0x0D)
		{
			// CR + LF
			if (i < length - 1 && text[i + 1] == 0x0A)
			{
				layoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
				rc.y += lineSize.height;
				rc.height -= lineSize.height;
				i++;
				begin = i + 1;
				m_result->AreaSize.width = std::max(m_result->AreaSize.width, lineSize.width);
				m_result->AreaSize.height += lineSize.height;
			}
			// CR
			else
			{
				layoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
				rc.y += lineSize.height;
				rc.height -= lineSize.height;
				begin = i;
				begin = i + 1;
				m_result->AreaSize.width = std::max(m_result->AreaSize.width, lineSize.width);
				m_result->AreaSize.height += lineSize.height;
			}
		}
	}
	if (begin != i)
	{
		layoutLineHorizontal(&text[begin], i - begin, rc, &lineSize);
		m_result->AreaSize.width = std::max(m_result->AreaSize.width, lineSize.width);
		m_result->AreaSize.height += lineSize.height;
	}

	// TODO: アンチエイリアス有効だと、↑の方法では1px 足りないことがあった。
	// とりあえずここで強制設定している。
	m_result->AreaSize.height = m_font->getLineSpacing();
}

//------------------------------------------------------------------------------
// lineArea : 行を描画できる領域。Y は左上の Y 座標。Bottom は描画領域の下端。
//------------------------------------------------------------------------------
void TextLayoutEngine::layoutLineHorizontal(const UTF32* text, int length, const RectI& lineArea, SizeI* outLineSize)
{
	outLineSize->set(0, 0);

	// 描く必要がない
	if (length == 0) { return; }

	// まずは左詰めで計測する
	FontGlyphLocation* prevInfo = NULL;
	//int planeWidth = 0;
	TextLayoutResultItem item;
	for (int i = 0; i < length; ++i)
	{
		prevInfo = m_font->advanceKerning(text[i], m_strokeSize, prevInfo);
		//planeWidth += prevInfo->BitmapSize.Width;
		outLineSize->height = std::max(outLineSize->height, prevInfo->BitmapSize.height);

		item.Char = text[i];
		item.Location = *prevInfo;


		if (m_layoutTextOptions == LayoutTextOptions::All)
		{
			m_result->Items.add(item);
		}
	}

	//int planeWidth = m_result->Items[length - 1].Location.OuterTopLeftPosition.X + m_result->Items[length - 1].Location.BitmapSize.Width;
	int planeWidth = item.Location.OuterTopLeftPosition.x + item.Location.BitmapSize.width;

	// TODO: GUI から呼ばれるときは、UI要素のサイズを図る目的でも使用する。
	// TextAlignment::Left 固定でいいのか考えておく。
	outLineSize->width = planeWidth;

	if (m_layoutTextOptions == LayoutTextOptions::All)
	{
		// 各グリフの配置を決める
		RectI drawArea = lineArea;
		switch (m_textAlignment)
		{
		case TextAlignment::Left:
		{
			break;
		}
		case TextAlignment::Center:
		{
			int offset = (lineArea.width - planeWidth) / 2;
			for (TextLayoutResultItem & item : m_result->Items) {
				item.Location.BitmapTopLeftPosition.x += offset;
				item.Location.OutlineBitmapTopLeftPosition.x += offset;
				item.Location.OuterTopLeftPosition.x += offset;
			}
			break;
		}
		case TextAlignment::Right:
		{
			int offset = lineArea.getRight() - planeWidth;
			for (TextLayoutResultItem & item : m_result->Items) {
				item.Location.BitmapTopLeftPosition.x += offset;
				item.Location.OutlineBitmapTopLeftPosition.x += offset;
				item.Location.OuterTopLeftPosition.x += offset;
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
}








//==============================================================================
// AbstractTextLayoutEngine
//==============================================================================
//------------------------------------------------------------------------------
void AbstractTextLayoutEngine::layout(RawFont* font, const UTF32* text, int length, const Rect& layoutArea, TextLayoutOptions options)
{
	m_font = font;
	m_options = options;

	m_font->getGlobalMetrics(&m_globalMetrics);
	m_currentLineBaseline = m_globalMetrics.ascender;

	layoutTextHorizontal(text, length);
}

//------------------------------------------------------------------------------
void AbstractTextLayoutEngine::layoutTextHorizontal(const UTF32* text, int length)
{
	const UTF32* lineBegin = text;
	const UTF32* end = text + length;
	while (lineBegin < end)
	{
		int nlPos, nlLen;
		if (StringTraits::indexOfNewLineSequence(lineBegin, end, &nlPos, &nlLen))
		{
			layoutLineHorizontal(lineBegin, nlPos);
			lineBegin += nlPos + nlLen;
			m_currentLineBaseline += m_globalMetrics.ascender;
		}
		else
		{
			break;
		}
	}
	if (lineBegin != end)
	{
		layoutLineHorizontal(lineBegin, end - lineBegin);
	}
}

//------------------------------------------------------------------------------
void AbstractTextLayoutEngine::layoutLineHorizontal(const UTF32* text, int length)
{
	ResultItem item;
	const UTF32* prev = nullptr;
	int x = 0;
	for (int i = 0; i < length; ++i)
	{
		if (prev != nullptr)
		{
			Vector2 delta = m_font->getKerning(*prev, text[i]);
			x -= delta.x;
		}

		item.ch = text[i];
		item.columnBaseline = x;
		item.lineBaseline = m_currentLineBaseline;
		onPlacementChar(item);

		FontGlyphMetrics metrics;
		m_font->getGlyphMetrics(text[i], &metrics);
		x += metrics.advance.x;

		prev = text + i;
	}
}

//==============================================================================
// TextLayoutEngine2
//==============================================================================
//------------------------------------------------------------------------------
void TextLayoutEngine2::layout(RawFont* font, const UTF32* text, int length, const Rect& layoutArea, TextLayoutOptions options, ResultData* outResult)
{
	m_result = outResult;
	m_result->areaSize = Size::Zero;
	m_result->items.clear();
	AbstractTextLayoutEngine::layout(font, text, length, layoutArea, options);
}

//------------------------------------------------------------------------------
void TextLayoutEngine2::onPlacementChar(const ResultItem& item)
{
	m_result->items.add(item);
}

} // namespace detail
LN_NAMESPACE_END

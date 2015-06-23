
/*
書式設定されたテキストの描画
https://msdn.microsoft.com/ja-jp/library/ms752098%28v=vs.110%29.aspx





*/

#include "../Internal.h"
#include <algorithm>
#include "TextRenderer.h"

namespace Lumino
{
namespace Imaging
{

//=============================================================================
// TextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::TextRenderer(Bitmap* targetBitmap, Font* font)
	: m_targetBitmap(targetBitmap)
	, m_font(font)
	, m_foreColor(Graphics::Color::Black)
	, m_strokeColor(Graphics::Color::Black)
	, m_strokeSize(0)
	, m_textAlignment(TextAlignment_Left)
	, m_textTrimming(TextTrimming_None)
	, m_flowDirection(FlowDirection_LeftToRight)
	, m_areaBox()
	, m_prevGlyphData(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const char* text, int length)
{
	LN_VERIFY_RETURN(text != NULL);
	if (length == 0) { return; }
	length = (length < 0) ? strlen(text) : length;

	// char → UTF32 に変換するのに必要なバイト数で領域確保
	size_t size = Text::Encoding::GetConversionRequiredByteCount(Text::Encoding::GetSystemMultiByteEncoding(), Text::Encoding::GetUTF32Encoding(), length);
	m_utf32Buffer.Resize(size);

	// UTF32 に変換
	Text::EncodingConversionResult result;
	Text::Encoding::Convert(
		text, length, Text::Encoding::GetSystemMultiByteEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Text::Encoding::GetUTF32Encoding(),
		&result);

	DrawTextHorizontal((UTF32*)m_utf32Buffer.GetData(), result.BytesUsed / sizeof(UTF32));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawText(const wchar_t* text, int length)
{
	LN_VERIFY_RETURN(text != NULL);
	if (length == 0) { return; }
	length = (length < 0) ? wcslen(text) : length;

	// char → UTF32 に変換するのに必要なバイト数で領域確保
	size_t size = Text::Encoding::GetConversionRequiredByteCount(Text::Encoding::GetWideCharEncoding(), Text::Encoding::GetUTF32Encoding(), length * sizeof(wchar_t));
	m_utf32Buffer.Resize(size);

	// UTF32 に変換
	Text::EncodingConversionResult result;
	Text::Encoding::Convert(
		text, length * sizeof(wchar_t), Text::Encoding::GetWideCharEncoding(),
		m_utf32Buffer.GetData(), m_utf32Buffer.GetSize(), Text::Encoding::GetUTF32Encoding(),
		&result);

	DrawTextHorizontal((UTF32*)m_utf32Buffer.GetData(), result.BytesUsed / sizeof(UTF32));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawTextHorizontal(const UTF32* text, int length)
{
	Rect lineArea(0, 0, m_areaBox.Width, m_font->GetLineHeight());
	int lineBegin = 0;
	int i = 0;
	for (; i < length; ++i)
	{
		// LF
		if (text[i] == 0x0A)
		{
			DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
			lineArea.Y += m_font->GetLineHeight();
			lineBegin = i;
			ResetKerning();	// 改行したのでカーニング情報をリセット
		}
		if (text[i] == 0x0D)
		{
			// CR + LF
			if (i < length - 1 && text[i + 1] == 0x0A)
			{
				DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
				lineBegin = i + 1;
			}
			// CR
			else
			{
				DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
				lineBegin = i;
			}
			lineArea.Y += m_font->GetLineHeight();
			ResetKerning();	// 改行したのでカーニング情報をリセット
		}
	}
	if (lineBegin != i)
	{
		DrawLineHorizontal(&text[lineBegin], i - lineBegin, lineArea);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextRenderer::DrawLineHorizontal(const UTF32* text, int length, const Rect& lineArea)
{
	// 描く必要がない
	if (text == NULL) { return; }
	if (length == 0) { return; }
	if (lineArea.Width <= 0 || lineArea.Height <= 0) { return; }

	Rect drawArea = lineArea;
	if (m_textAlignment == TextAlignment_Center)
	{
		// 中央揃え
		Size size = m_font->GetTextSize(text, length);
		drawArea.X += (drawArea.Width - size.Width) / 2;
	}
	else if (m_textAlignment == TextAlignment_Right)
	{
		// 右揃え
		Size size = m_font->GetTextSize(text, length);
		drawArea.X = (drawArea.X + drawArea.Width) - size.Width;
	}
	else if (m_textAlignment == TextAlignment_Justify)
	{
		// 両端揃え
		LN_THROW(0, NotImplementedException);
	}

	// 描画ループ
	for (int i = 0; i < length; ++i)
	{
		FontGlyphData* glyphData = m_font->LookupGlyphData(text[i], m_prevGlyphData);

		// グリフビットマップの転送先
		Rect destRect(
			drawArea.X + glyphData->GlyphOffsetX,
			drawArea.Y + glyphData->GlyphOffsetY,
			drawArea.Width - glyphData->GlyphOffsetX,
			drawArea.Height - glyphData->GlyphOffsetY);

		// アウトライン用のビットマップがあれば先に描く
		if (glyphData->OutlineBitmap)
		{
			m_targetBitmap->BitBlt(
				destRect,
				glyphData->OutlineBitmap,
				Rect(0, 0, glyphData->OutlineBitmap->GetSize()),
				m_strokeColor,
				true);

			destRect.X += glyphData->OutlineOffset;
			destRect.Y += glyphData->OutlineOffset;
		}

		// グリフビットマップを描画
		m_targetBitmap->BitBlt(
			destRect,
			glyphData->GlyphBitmap,
			Rect(0, 0, glyphData->GlyphBitmap->GetSize()),
			m_foreColor,
			true);

		m_prevGlyphData = glyphData;
	}
}

} // namespace Imaging
} // namespace Lumino

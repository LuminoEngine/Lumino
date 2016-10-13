
#include "../Internal.h"
#include <Lumino/Graphics/Bitmap.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "BitmapTextRenderer.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// BitmapTextRenderer
//==============================================================================

//------------------------------------------------------------------------------
BitmapTextRenderer::BitmapTextRenderer()
	: m_manager(nullptr)
	, m_tmpGlyphRun(nullptr)
	, m_textAlignment(TextAlignment::Left)
{
}

//------------------------------------------------------------------------------
BitmapTextRenderer::~BitmapTextRenderer()
{
	LN_SAFE_RELEASE(m_tmpGlyphRun);
}

//------------------------------------------------------------------------------
void BitmapTextRenderer::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;

	m_tmpGlyphRun = LN_NEW GlyphRun();
	m_tmpGlyphRun->Initialize(m_manager);
}

//------------------------------------------------------------------------------
void BitmapTextRenderer::DrawGlyphRun(Bitmap* target, GlyphRun* glyphRun, const Color32& fillColor, const Color32& strokeColor, int strokeThickness)
{
	LN_CHECK_ARG(target != nullptr);
	LN_CHECK_ARG(glyphRun != nullptr);

	RawFont* font = glyphRun->GetFont();

	auto& items = glyphRun->RequestLayoutItems();
	auto& renderSize = glyphRun->GetRenderSize();

	Point offset(0, 0);
	switch (m_textAlignment)
	{
		case TextAlignment::Left:
		{
			break;
		}
		case TextAlignment::Center:
		{
			offset.x = (m_renderArea.width - renderSize.width) / 2;
			break;
		}
		case TextAlignment::Right:
		{
			offset.x = m_renderArea.GetRight() - renderSize.width;
			break;
		}
		case TextAlignment::Justify:
			LN_THROW(0, NotImplementedException);
			break;
	}


	Point pos(m_renderArea.x + offset.x, m_renderArea.y + offset.y);
	for (auto& item : items)
	{
		FontGlyphBitmap* gb = font->LookupGlyphBitmap(item.Char, strokeThickness);
		Rect dstRect;
		Rect srcRect;

		// 枠線用ビットマップがある場合は先に描画する
		if (gb->OutlineBitmap != nullptr)
		{
			dstRect.Set(
				pos.x + item.Location.OutlineBitmapTopLeftPosition.x,
				pos.y + item.Location.OutlineBitmapTopLeftPosition.y,
				item.Location.BitmapSize.width,
				item.Location.BitmapSize.height);
			srcRect.Set(0, 0, gb->OutlineBitmap->GetSize());
			target->BitBlt(dstRect, gb->OutlineBitmap, srcRect, strokeColor, true);
		}

		// 内側 (or 通常) 部分の描画
		if (gb->GlyphBitmap != nullptr)
		{
			dstRect.Set(
				pos.x + item.Location.BitmapTopLeftPosition.x,
				pos.y + item.Location.BitmapTopLeftPosition.y,
				item.Location.BitmapSize.width,
				item.Location.BitmapSize.height);
			srcRect.Set(0, 0, gb->GlyphBitmap->GetSize());
			target->BitBlt(dstRect, gb->GlyphBitmap, srcRect, fillColor, true);
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

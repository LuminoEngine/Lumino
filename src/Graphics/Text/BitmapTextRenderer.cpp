
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "BitmapTextRenderer.h"
#include "../GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// BitmapTextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapTextRenderer::BitmapTextRenderer()
	: m_manager(nullptr)
	, m_tmpGlyphRun(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapTextRenderer::~BitmapTextRenderer()
{
	LN_SAFE_RELEASE(m_tmpGlyphRun);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapTextRenderer::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;

	m_tmpGlyphRun = LN_NEW GlyphRun();
	m_tmpGlyphRun->Initialize(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapTextRenderer::DrawGlyphRun(Bitmap* target, GlyphRun* glyphRun, const Color& fillColor, const Color& strokeColor, int strokeThickness)
{
	LN_CHECK_ARGS_RETURN(target != nullptr);
	LN_CHECK_ARGS_RETURN(glyphRun != nullptr);

	Font* font = glyphRun->GetFont();

	auto& items = glyphRun->RequestLayoutItems();
	for (auto& item : items)
	{
		FontGlyphBitmap* gb = font->LookupGlyphBitmap(item.Char, strokeThickness);
		Rect dstRect;
		Rect srcRect;

		// 枠線用ビットマップがある場合は先に描画する
		if (gb->OutlineBitmap != nullptr)
		{
			dstRect.Set(
				item.Location.OutlineBitmapTopLeftPosition.X,
				item.Location.OutlineBitmapTopLeftPosition.Y,
				item.Location.BitmapSize.Width,
				item.Location.BitmapSize.Height);
			srcRect.Set(0, 0, gb->OutlineBitmap->GetSize());
			target->BitBlt(dstRect, gb->OutlineBitmap, srcRect, strokeColor, true);
		}

		// 内側 (or 通常) 部分の描画
		if (gb->GlyphBitmap != nullptr)
		{
			dstRect.Set(
				item.Location.BitmapTopLeftPosition.X,
				item.Location.BitmapTopLeftPosition.Y,
				item.Location.BitmapSize.Width,
				item.Location.BitmapSize.Height);
			srcRect.Set(0, 0, gb->GlyphBitmap->GetSize());
			target->BitBlt(dstRect, gb->GlyphBitmap, srcRect, fillColor, true);
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

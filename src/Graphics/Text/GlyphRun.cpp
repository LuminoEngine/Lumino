
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../GraphicsManager.h"
#include "FontGlyphTextureCache.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// BitmapTextRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GlyphRun::GlyphRun()
	: m_manager(nullptr)
	, m_layoutEngine(nullptr)
	, m_glyphTextureCache(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GlyphRun::~GlyphRun()
{
	LN_SAFE_RELEASE(m_glyphTextureCache);
	LN_SAFE_DELETE(m_layoutEngine);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS(manager != nullptr);
	m_manager = manager;
	m_layoutEngine = LN_NEW TextLayoutEngine();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::SetFont(Font* font)
{
	if (font != m_layoutEngine->GetFont())
	{
		m_layoutEngine->SetFont(font);
		UpdateTextLayoutItem();
		LN_SAFE_RELEASE(m_glyphTextureCache);	// 必要なときにまた取得しなおす
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Font* GlyphRun::GetFont() const
{
	return m_layoutEngine->GetFont();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::SetText(const StringRef& text)
{
	EncodingConverter* conv = m_manager->GetFontManager()->GetTCharToUTF32Converter();
	m_utf32Text.Clear();
	m_utf32Text.Append(conv->Convert(text.GetBegin(), text.GetLength() * sizeof(TCHAR)));
	UpdateTextLayoutItem();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::UpdateTextLayoutItem()
{
	m_glyphData.AreaSize = Size::Zero;
	m_glyphData.Items.Clear();
	if (m_layoutEngine->GetFont() != nullptr && m_utf32Text.GetLength() > 0)
	{
		m_layoutEngine->LayoutText(m_utf32Text.c_str(), m_utf32Text.GetLength(), &m_glyphData);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Array<TextLayoutResultItem>& GlyphRun::GetLayoutItems() const
{
	return m_glyphData.Items;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Internal::FontGlyphTextureCache* GlyphRun::LookupFontGlyphTextureCache()
{
	if (m_glyphTextureCache == nullptr)
	{
		m_glyphTextureCache = m_manager->LookupGlyphTextureCache(m_layoutEngine->GetFont());	// 参照カウントは増えている
	}
	return m_glyphTextureCache;
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::Update(const UTF32* text, int length)
{
	m_glyphData->Items.Clear();
	m_glyphTextureCache->Measure(text, length, m_glyphData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size GlyphRun::GetRenderSize() const
{
	return m_glyphData->AreaSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GlyphRun::AttachGlyphTextureCache(Internal::FontGlyphTextureCache* cache)
{
	LN_REFOBJ_SET(m_glyphTextureCache, cache);
}
#endif

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

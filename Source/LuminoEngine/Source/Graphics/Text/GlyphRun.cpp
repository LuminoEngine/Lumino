
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../GraphicsManager.h"
#include "FontManager.h"
#include "BitmapTextRenderer.h"
#include "FontGlyphTextureCache.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// GlyphRun
//		主な目的は文字配置の再計算を行わないようにすること。
//		カーニングの計算にも検索処理が必要となる。
//==============================================================================

//------------------------------------------------------------------------------
GlyphRun::GlyphRun()
	: m_manager(nullptr)
	, m_layoutEngine(nullptr)
	, m_glyphTextureCache(nullptr)
	, m_modifiedRenderSize(false)
	, m_modifiedItems(false)
{
}

//------------------------------------------------------------------------------
GlyphRun::~GlyphRun()
{
	LN_SAFE_RELEASE(m_glyphTextureCache);
	LN_SAFE_DELETE(m_layoutEngine);
}

//------------------------------------------------------------------------------
void GlyphRun::initialize()
{
	initialize(detail::EngineDomain::GetGraphicsManager());
}

//------------------------------------------------------------------------------
void GlyphRun::initialize(detail::GraphicsManager* manager)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_manager = manager;
	m_layoutEngine = LN_NEW detail::TextLayoutEngine();
	m_layoutEngine->SetFont(m_manager->GetFontManager()->GetDefaultRawFont());
}

//------------------------------------------------------------------------------
void GlyphRun::SetFont(RawFont* font)
{
	if (font != m_layoutEngine->GetFont())
	{
		if (font == nullptr)
		{
			font = m_manager->GetFontManager()->GetDefaultRawFont();
		}
		m_layoutEngine->SetFont(font);
		LN_SAFE_RELEASE(m_glyphTextureCache);	// 必要なときにまた取得しなおす
		m_modifiedRenderSize = true;
		m_modifiedItems = true;
	}
}

//------------------------------------------------------------------------------
RawFont* GlyphRun::GetFont() const
{
	return m_layoutEngine->GetFont();
}

//------------------------------------------------------------------------------
void GlyphRun::setText(const UTF32* str, int len)
{
	m_utf32Text.clear();
	m_utf32Text.append(str, len);
	m_modifiedRenderSize = true;
	m_modifiedItems = true;
}

//------------------------------------------------------------------------------
void GlyphRun::setText(const StringRef& text)
{
	EncodingConverter* conv = m_manager->GetFontManager()->GetTCharToUTF32Converter();
	m_utf32Text.clear();
	m_utf32Text.append(conv->Convert(text.getBegin(), text.getLength() * sizeof(TCHAR)));
	m_modifiedRenderSize = true;
	m_modifiedItems = true;
}

//------------------------------------------------------------------------------
void GlyphRun::SetTextAlignment(TextAlignment align)
{
	m_layoutEngine->SetTextAlignment(align);
}

//------------------------------------------------------------------------------
//int GlyphRun::GetStrokeSize() const
//{
//	return m_layoutEngine->GetStrokeSize();
//}

//------------------------------------------------------------------------------
const SizeI& GlyphRun::GetRenderSize()
{
	// RenderSize だけ更新する
	if (m_modifiedRenderSize)
	{
		if (m_layoutEngine->GetFont() != nullptr && m_utf32Text.getLength() > 0)
		{
			m_layoutEngine->LayoutText(m_utf32Text.c_str(), m_utf32Text.getLength(), detail::LayoutTextOptions::RenderSizeOnly, &m_glyphData);
		}
		m_modifiedRenderSize = false;
	}
	return m_glyphData.AreaSize;
}

//------------------------------------------------------------------------------
bool GlyphRun::GetCharacterHitFromDistance(const PointF& pos, GlyphHit* outResult)
{
	UpdateTextLayoutItem();

	Size area = m_glyphData.AreaSize.ToFloatSize();
	if (0.0f <= pos.x && pos.x <= area.width &&		// TODO: Contains とかの関数にしたい
		0.0f <= pos.y && pos.y <= area.height)
	{
		for (auto& item : m_glyphData.Items)
		{
			if (item.Location.BitmapTopLeftPosition.x <= pos.x && pos.x <= item.Location.BitmapTopLeftPosition.x + item.Location.BitmapSize.width &&
				item.Location.BitmapTopLeftPosition.y <= pos.y && pos.y <= item.Location.BitmapTopLeftPosition.y + item.Location.BitmapSize.height)
			{
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool GlyphRun::GetDistanceFromCharacterHit(int index, PointF* outPos)
{
	UpdateTextLayoutItem();

	if (index < 0 || m_glyphData.Items.isEmpty())
	{
		outPos->Set(0, 0);
	}
	else if (index >= m_glyphData.Items.getCount())
	{
		outPos->x = m_glyphData.Items.getLast().Location.BitmapTopLeftPosition.x + m_glyphData.Items.getLast().Location.BitmapSize.width;
		outPos->y = 0;
	}
	else
	{
		outPos->x = m_glyphData.Items[index].Location.BitmapTopLeftPosition.x;
		outPos->y = 0;
	}
	return true;
}

//------------------------------------------------------------------------------
void GlyphRun::UpdateTextLayoutItem()
{
	if (m_modifiedItems)
	{
		m_glyphData.AreaSize = SizeI::Zero;
		m_glyphData.Items.clear();
		if (m_layoutEngine->GetFont() != nullptr && m_utf32Text.getLength() > 0)
		{
			m_layoutEngine->LayoutText(m_utf32Text.c_str(), m_utf32Text.getLength(), detail::LayoutTextOptions::All, &m_glyphData);
		}
		m_modifiedRenderSize = false;
		m_modifiedItems = false;
	}
}

//------------------------------------------------------------------------------
const List<TextLayoutResultItem>& GlyphRun::RequestLayoutItems()
{
	UpdateTextLayoutItem();
	return m_glyphData.Items;
}

//------------------------------------------------------------------------------
detail::FontGlyphTextureCache* GlyphRun::LookupFontGlyphTextureCache()
{
	return m_layoutEngine->GetFont()->GetGlyphTextureCache();
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

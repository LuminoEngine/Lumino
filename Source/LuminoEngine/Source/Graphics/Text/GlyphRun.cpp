
#include "../Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../GraphicsManager.h"
#include "FontManager.h"
#include "BitmapTextRenderer.h"
#include "FontGlyphTextureCache.h"

LN_NAMESPACE_BEGIN

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
	initialize(detail::EngineDomain::getGraphicsManager());
}

//------------------------------------------------------------------------------
void GlyphRun::initialize(detail::GraphicsManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
	m_layoutEngine = LN_NEW detail::TextLayoutEngine();
	m_layoutEngine->setFont(m_manager->getFontManager()->getDefaultRawFont());
}

//------------------------------------------------------------------------------
void GlyphRun::setFont(RawFont* font)
{
	if (font != m_layoutEngine->getFont())
	{
		if (font == nullptr)
		{
			font = m_manager->getFontManager()->getDefaultRawFont();
		}
		m_layoutEngine->setFont(font);
		LN_SAFE_RELEASE(m_glyphTextureCache);	// 必要なときにまた取得しなおす
		m_modifiedRenderSize = true;
		m_modifiedItems = true;
	}
}

//------------------------------------------------------------------------------
RawFont* GlyphRun::getFont() const
{
	return m_layoutEngine->getFont();
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
	EncodingConverter* conv = m_manager->getFontManager()->getTCharToUTF32Converter();
	m_utf32Text.clear();
	m_utf32Text.append(conv->convert(text.getBegin(), text.getLength() * sizeof(Char)));
	m_modifiedRenderSize = true;
	m_modifiedItems = true;
}

//------------------------------------------------------------------------------
void GlyphRun::setTextAlignment(TextAlignment align)
{
	m_layoutEngine->setTextAlignment(align);
}

//------------------------------------------------------------------------------
//int GlyphRun::getStrokeSize() const
//{
//	return m_layoutEngine->getStrokeSize();
//}

//------------------------------------------------------------------------------
const SizeI& GlyphRun::getRenderSize()
{
	// RenderSize だけ更新する
	if (m_modifiedRenderSize)
	{
		if (m_layoutEngine->getFont() != nullptr && m_utf32Text.getLength() > 0)
		{
			m_layoutEngine->layoutText(m_utf32Text.c_str(), m_utf32Text.getLength(), detail::LayoutTextOptions::RenderSizeOnly, &m_glyphData);
		}
		m_modifiedRenderSize = false;
	}
	return m_glyphData.AreaSize;
}

//------------------------------------------------------------------------------
bool GlyphRun::getCharacterHitFromDistance(const Point& pos, GlyphHit* outResult)
{
	updateTextLayoutItem();

	Size area = m_glyphData.AreaSize.toFloatSize();
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
bool GlyphRun::getDistanceFromCharacterHit(int index, Point* outPos)
{
	updateTextLayoutItem();

	if (index < 0 || m_glyphData.Items.isEmpty())
	{
		outPos->set(0, 0);
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
void GlyphRun::updateTextLayoutItem()
{
	if (m_modifiedItems)
	{
		m_glyphData.AreaSize = SizeI::Zero;
		m_glyphData.Items.clear();
		if (m_layoutEngine->getFont() != nullptr && m_utf32Text.getLength() > 0)
		{
			m_layoutEngine->layoutText(m_utf32Text.c_str(), m_utf32Text.getLength(), detail::LayoutTextOptions::All, &m_glyphData);
		}
		m_modifiedRenderSize = false;
		m_modifiedItems = false;
	}
}

//------------------------------------------------------------------------------
const List<TextLayoutResultItem>& GlyphRun::requestLayoutItems()
{
	updateTextLayoutItem();
	return m_glyphData.Items;
}

//------------------------------------------------------------------------------
detail::FontGlyphTextureCache* GlyphRun::lookupFontGlyphTextureCache()
{
	return m_layoutEngine->getFont()->GetGlyphTextureCache();
}

LN_NAMESPACE_END

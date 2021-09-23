
#include "Internal.hpp"
#include "FontManager.hpp"
#include "FontCore.hpp"

namespace ln {
namespace detail {

//==============================================================================
// FontCore
	
FontCore::FontCore()
	: m_manager(nullptr)
{
}

void FontCore::init(FontManager* manager)
{
	if (LN_REQUIRE(manager)) return;
	m_manager = manager;
	m_manager->addAliveFontCore(this);
}

void FontCore::dispose()
{
	if (m_manager)
	{
		m_manager->removeAliveFontCore(this);
		m_manager = nullptr;
	}
}

void FontCore::finalize()
{
	// ObjectCache から削除されるとき用
	dispose();
}

//FontGlyphTextureCache* FontCore::getFontGlyphTextureCache()
//{
//	if (!m_fontGlyphTextureCache)
//	{
//		m_fontGlyphTextureCache = makeRef<FontGlyphTextureCache>();
//		m_fontGlyphTextureCache->init(this);
//	}
//	return m_fontGlyphTextureCache;
//}


} // namespace detail
} // namespace ln


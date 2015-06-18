
#include "../Internal.h"
#include <ft2build.h>
#include FT_FREETYPE_H	/* <freetype/freetype.h> */
#include FT_CACHE_H	/* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H	/* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H	/* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include "FontManagerImpl.h"
#include <Lumino/Imaging/FontManager.h>

namespace Lumino
{
namespace Imaging
{
	
//=============================================================================
// FontManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::FontManager(FileManager* fileManager)
{
	m_fileManager = fileManager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::~FontManager()
{
}

} // namespace Imaging
} // namespace Lumino

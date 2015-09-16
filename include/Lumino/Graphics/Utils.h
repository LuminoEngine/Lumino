
#pragma once

#include "../Graphics/Bitmap.h"
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		
*/
class Utils
{
public:

	static PixelFormat TranslatePixelFormat(TextureFormat textureFormat);
	
	static void SaveScreenShot(const TCHAR* filePath);
	static void WaitSaveScreenShot();
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino

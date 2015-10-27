
#pragma once

#include "../Graphics/Bitmap.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		
*/
class Utils
{
public:

	static int GetTextureFormatByteCount(TextureFormat format);
	static PixelFormat TranslatePixelFormat(TextureFormat format);
	
	static void SaveScreenShot(const TCHAR* filePath);
	static void WaitSaveScreenShot();
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

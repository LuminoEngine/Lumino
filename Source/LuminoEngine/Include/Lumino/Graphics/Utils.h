
#pragma once

#include "../Graphics/Bitmap.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
class Texture;

/**
	@brief		
*/
class Utils
{
public:
	static IndexBufferFormat getIndexBufferFormat(int indexCount);
	static int getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat_UInt16) ? 2 : 4; }
	static int getIndexBufferSize(IndexBufferFormat format, int indexCount) { return getIndexStride(format) * indexCount; }

	static int getTextureFormatByteCount(TextureFormat format);
	static PixelFormat translatePixelFormat(TextureFormat format);

	static bool isSRGBFormat(TextureFormat format);
	static bool isFloatFormat(TextureFormat format);
	
	static void saveScreenShot(const Char* filePath);
	static void waitSaveScreenShot();
};

LN_NAMESPACE_END

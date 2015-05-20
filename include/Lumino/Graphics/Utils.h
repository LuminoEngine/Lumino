
#pragma once

#include "../Imaging/Bitmap.h"
#include "Common.h"

namespace Lumino
{
namespace Graphics
{

/**
	@brief		
*/
class Utils
{
public:

	static Imaging::PixelFormat TranslatePixelFormat(TextureFormat textureFormat);
	
	static void SaveScreenShot(const TCHAR* filePath);
	static void WaitSaveScreenShot();
};

} // namespace Graphics
} // namespace Lumino

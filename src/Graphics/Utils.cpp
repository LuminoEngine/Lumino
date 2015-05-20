
#pragma once

#include "../Internal.h"
#include <Lumino/Graphics/Utils.h>

namespace Lumino
{
namespace Graphics
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Imaging::PixelFormat Utils::TranslatePixelFormat(TextureFormat textureFormat)
{
	static const Imaging::PixelFormat table[] =
	{
		Imaging::PixelFormat_Unknown,		// TextureFormat_Unknown = 0,

		Imaging::PixelFormat_BYTE_R8G8B8A8,	// TextureFormat_R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
		Imaging::PixelFormat_Unknown,		// TextureFormat_R8G8B8X8,            ///< 32 ビットのアルファ無しフォーマット

		Imaging::PixelFormat_BYTE_B8G8R8A8,	// TextureFormat_B8G8R8A8,            ///< 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。D3D_FMT_A8R8G8B8, DXGI_FORMAT_B8G8R8A8_UNORM)
		Imaging::PixelFormat_BYTE_B8G8R8X8,	// TextureFormat_B8G8R8X8,            ///< 32 ビットのアルファ無しフォーマット

		Imaging::PixelFormat_Unknown,		// TextureFormat_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
		Imaging::PixelFormat_Unknown,		// TextureFormat_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
		Imaging::PixelFormat_Unknown,		// TextureFormat_D24S8,               ///< 32 ビットの Z バッファフォーマット
		Imaging::PixelFormat_Unknown,		// TextureFormat_R16F,
		Imaging::PixelFormat_Unknown,		// TextureFormat_R32F,
	};
	assert(LN_ARRAY_SIZE_OF(table) == TextureFormat_Max);
	return table[textureFormat];
}

} // namespace Graphics
} // namespace Lumino

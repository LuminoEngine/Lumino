
#pragma once

#include "../Internal.h"
#include <Lumino/Graphics/Utils.h>

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
	
//=============================================================================
// Utils
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Utils::GetTextureFormatByteCount(TextureFormat format)
{
	static const int table[] =
	{
		0,		// TextureFormat_Unknown = 0,
		4,		// TextureFormat_R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
		4,		// TextureFormat_R8G8B8X8,            ///< 32 ビットのアルファ無しフォーマット
		4,		// TextureFormat_B8G8R8A8,            ///< 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。D3D_FMT_A8R8G8B8, DXGI_FORMAT_B8G8R8A8_UNORM)
		4,		// TextureFormat_B8G8R8X8,            ///< 32 ビットのアルファ無しフォーマット
		8,		// TextureFormat_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
		16,		// TextureFormat_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
		4,		// TextureFormat_D24S8,               ///< 32 ビットの Z バッファフォーマット
		2,		// TextureFormat_R16F,
		4,		// TextureFormat_R32F,
	};
	assert(LN_ARRAY_SIZE_OF(table) == TextureFormat_Max);
	return table[format];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PixelFormat Utils::TranslatePixelFormat(TextureFormat format)
{
	static const PixelFormat table[] =
	{
		PixelFormat_Unknown,		// TextureFormat_Unknown = 0,

		PixelFormat_BYTE_R8G8B8A8,	// TextureFormat_R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
		PixelFormat_Unknown,		// TextureFormat_R8G8B8X8,            ///< 32 ビットのアルファ無しフォーマット

		PixelFormat_BYTE_B8G8R8A8,	// TextureFormat_B8G8R8A8,            ///< 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。D3D_FMT_A8R8G8B8, DXGI_FORMAT_B8G8R8A8_UNORM)
		PixelFormat_BYTE_B8G8R8X8,	// TextureFormat_B8G8R8X8,            ///< 32 ビットのアルファ無しフォーマット

		PixelFormat_Unknown,		// TextureFormat_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
		PixelFormat_R32G32B32A32_Float,		// TextureFormat_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
		PixelFormat_Unknown,		// TextureFormat_D24S8,               ///< 32 ビットの Z バッファフォーマット
		PixelFormat_Unknown,		// TextureFormat_R16F,
		PixelFormat_Unknown,		// TextureFormat_R32F,
	};
	assert(LN_ARRAY_SIZE_OF(table) == TextureFormat_Max);
	return table[format];
}

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino

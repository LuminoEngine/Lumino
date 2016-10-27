
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Utils.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
	
//==============================================================================
// Utils
//==============================================================================

//------------------------------------------------------------------------------
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
	assert(LN_ARRAY_SIZE_OF(table) == (int)TextureFormat::_Count);
	return table[(int)format];
}

//------------------------------------------------------------------------------
PixelFormat Utils::TranslatePixelFormat(TextureFormat format)
{
	static const PixelFormat table[] =
	{
		PixelFormat::Unknown,		// TextureFormat_Unknown = 0,

		PixelFormat::R8G8B8A8,	// TextureFormat_R8G8B8A8,            ///< 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
		PixelFormat::Unknown,		// TextureFormat_R8G8B8X8,            ///< 32 ビットのアルファ無しフォーマット

		PixelFormat::B8G8R8A8,	// TextureFormat_B8G8R8A8,            ///< 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。D3D_FMT_A8R8G8B8, DXGI_FORMAT_B8G8R8A8_UNORM)
		PixelFormat::B8G8R8X8,	// TextureFormat_B8G8R8X8,            ///< 32 ビットのアルファ無しフォーマット

		PixelFormat::Unknown,		// TextureFormat_A16B16G16R16F,       ///< 64 ビットの浮動小数点フォーマット
		PixelFormat::FloatR32G32B32A32,		// TextureFormat_A32B32G32R32F,       ///< 128 ビットの浮動小数点フォーマット
		PixelFormat::Unknown,		// TextureFormat_D24S8,               ///< 32 ビットの Z バッファフォーマット
		PixelFormat::Unknown,		// TextureFormat_R16F,
		PixelFormat::Unknown,		// TextureFormat_R32F,
	};
	assert(LN_ARRAY_SIZE_OF(table) == (int)TextureFormat::_Count);
	return table[(int)format];
}

//------------------------------------------------------------------------------
bool Utils::EqualsTexture(Texture* texture1, Texture* texture2)
{
	if (texture1 != texture2) return false;
	if (texture1 != nullptr)
	{
		if (texture1->GetDeviceObject() != texture2->GetDeviceObject()) return false;
	}
	return true;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

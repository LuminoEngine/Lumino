
#pragma once

namespace ln {
class Bitmap2D;

/** ピクセルフォーマット */
LN_ENUM()
enum class PixelFormat : uint8_t
{
	/** Unknown */
	Unknown,

	/** 8bit アルファ値のみのフォーマット */
	A8,

	/** RGBA オーダーの各要素 8bit フォーマット */
	RGBA8,

	/** RGB オーダーの各要素 8bit フォーマット */
	RGB8,

	/** RGBA オーダーの各要素 32bit 浮動小数点フォーマット */
	RGBA32F,

	R32S,
};

} // namespace ln

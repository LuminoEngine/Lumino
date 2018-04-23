#pragma once

namespace ln {

enum class ClearFlags
{
	None	= 0x0000,
	Color	= 0x0001,					///< カラーバッファをクリアします。
	Depth	= 0x0002,					///< 深度バッファをクリアします。
	Stencil = 0x0004,					///< ステンシルバッファをクリアします。
	All		= Color | Depth | Stencil,	///< 全てのバッファをクリアします。
};
LN_FLAGS_OPERATORS(ClearFlags);

} // namespace ln

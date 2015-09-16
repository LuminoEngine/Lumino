
#pragma once 

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

/// テクスチャフィルタ
enum TextureFilterMode
{
	TextureFilterMode_Point = 0,		///< 補間を行わない
	TextureFilterMode_Linear,			///< 補間を行う
};

/// テクスチャアドレッシング
enum TextureWrapMode
{
	TextureWrapMode_Repeat = 0,		///< 繰り返し
	TextureWrapMode_Clamp,			///< 境界のピクセルが引き延ばされる
};

/**
	@brief	テクスチャのサンプリングステートを表すクラスです。
	@note	DirectX 等のグラフィックスAPI レベルでは、Filter は Min/Mag/Mip の3種、
			Wrap は U、V 方向の2種設定できる。
			しかし、実際のケースとしてそこまで細かく設定する必要があるのか？という考えや
			Unity 等のゲームエンジンでは細かくしていないことから、実装をシンプルにするために
			項目を減らしている。
*/
class SamplerState
{
public:

	TextureFilterMode	FilterMode;		///< テクスチャフィルタ(default:TextureFilterMode_Point)
	TextureWrapMode		WrapMode;		///< テクスチャアドレッシング(default:TextureWrapMode_Repeat)

public:

	/**
		@brief	デフォルト値で初期化します。
	*/
	SamplerState();
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino

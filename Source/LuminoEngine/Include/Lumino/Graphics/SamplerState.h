
#pragma once 

LN_NAMESPACE_BEGIN

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

LN_NAMESPACE_END

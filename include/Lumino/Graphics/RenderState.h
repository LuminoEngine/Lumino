
#pragma once 

namespace Lumino
{
namespace Graphics
{

/// 合成方法を表します
enum BlendMode
{
	BlendMode_Normal = 0,		///< 通常 (アルファ無視)
	BlendMode_Alpha,			///< アルファブレンド
	BlendMode_Add,				///< 加算
	BlendMode_AddAlphaDisable,	///< 加算（アルファ無視）
	BlendMode_Sub,				///< 減算
	BlendMode_SubAlphaDisable,	///< 減算（アルファ無視）
	BlendMode_Mul,				///< 乗算
	BlendMode_Screen,			///< スクリーン
	//BlendMode_Reverse,			///< リバース

	BlendMode_Max,				///< (terminator)
};

/// カリング方法を表します
enum CullingMode
{
	CullingMode_None = 0,		///< 両面表示
	CullingMode_Front,			///< 前面を描画しない
	CullingMode_Back,			///< 裏面を描画しない

	CullingMode_Max,			///< (terminator)
};

/// 塗りつぶし方法を表します
enum FillMode
{
	FillMode_Solid = 0,			///< 面を塗りつぶす
	FillMode_Wire,				///< ワイヤーフレーム
	FillMode_Point,				///< 頂点

	FillMode_Max,				///< (terminator)
};

/**
	@brief	グラフィックデバイスのレンダリングステートを表すクラスです。
*/
class RenderState
{
public:

	BlendMode		Blend;			///< 合成方法 (default:BlendMode_Normal)
	CullingMode		Culling;		///< カリング方法 (default:CullingMode_Back)
	FillMode		Fill;			///< 塗りつぶし方法 (default:FillMode_Solid。Opengl ES では無効)
	bool			AlphaTest;      ///< アルファテストの有無 (default:true。Opengl ES では無効)
	bool			DepthTest;		///< 深度テストの有無 (default:true)
	bool			DepthWrite;		///< 深度書き込みの有無 (default:true)
	//bool			PointSprite;	///< ポイントスプライトモードの有無 (default:false)

public:

	/**
		@brief	デフォルト値で初期化します。
	*/
	RenderState();

	/**
		@brief	現在の設定のハッシュ値を計算します。
	*/
	uint32_t GetHashCode() const;

public:
	bool operator == (const RenderState& obj) const;
	bool operator != (const RenderState& obj) const { return !operator==(obj); }
};

} // namespace Graphics
} // namespace Lumino

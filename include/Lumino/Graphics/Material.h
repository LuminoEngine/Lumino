
#pragma once

#include "Color.h"
#include "Texture.h"
#include "Shader.h"

namespace Lumino
{
namespace Graphics
{
/**
	@brief	マテリアルのクラスです。
*/
class Material
{
public:

	/// 描画オプション
	enum DrawingFlags
	{
		DrawingFlag_CullingDouble = 0x01,	///< 両面描画
		DrawingFlag_GroundShadow = 0x02,	///< 地面影
		DrawingFlag_SelfShadowMap = 0x04,	///< セルフシャドウマップへの描画
		DrawingFlag_SelfShadow = 0x08,		///< セルフシャドウの描画
		DrawingFlag_Edge = 0x10,			///< エッジ描画
	};

	/// スフィアテクスチャの合成モード
	enum SphereMode
	{
		SphereMode_Disable = 0,				///< 無効
		SphereMode_Mul = 1,					///< 乗算(sph)
		SphereMode_Add = 2,					///< 加算(spa)
		SphereMode_SubTexture = 3,			///< サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
	};

public:
	ColorF						Diffuse;			///< 物体の色
	ColorF						Ambient;			///< 環境光
	ColorF						Specular;			///< 光沢
	ColorF						Emissive;			///< 物体の発光色 ( 光源の影響を受けない色 )
	float						Power;				///< 光沢の強さ

	RefPtr<Graphics::Shader>	Shader;				///< シェーダ
	RefPtr<Graphics::Texture>	Texture;			///< テクスチャ
	RefPtr<Graphics::Texture>	ToonTexture;		///< [PMD] トゥーンテクスチャ (getToonTexture() setToonTexture()でアクセスする)
	RefPtr<Graphics::Texture>	SphereTexture;		///< [PMD] スフィアテクスチャ (getSphereTexture() setSphereTexture()でアクセスする)

	ColorF						ToonColor;			///< [PMD] トゥーンカラー
	ColorF						EdgeColor;			///< [PMX] エッジカラー
	float						EdgeSize;			///< [PMX] エッジサイズ
	ColorF						TextureCoe;			///< [PMX] テクスチャ係数
	ColorF						SphereTextureCoe;	///< [PMX] スフィアテクスチャ係数
	ColorF						ToonTextureCoe;		///< [PMX] Toonテクスチャ係数
	uint32_t					DrawingFlags;		///< [PMX] 描画オプション (MMDDrawingFlags の組み合わせ)
	SphereMode					SphereMode;			///< [PMX] スフィアテクスチャの合成モード

public:

	/**
		@brief		マテリアルを初期化します。
	*/
	Material()
	{
		Diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
		Ambient.Set(0.0f, 0.0f, 0.0f, 0.0f);
		Specular.Set(0.5f, 0.5f, 0.5f, 0.5f);
		Emissive.Set(0.0f, 0.0f, 0.0f, 0.0f);
		Power = 50.0f;
		ToonColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		EdgeColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		EdgeSize = 0.0f;
		SphereMode = SphereMode_Disable;
		DrawingFlags = 0;
	}
};

} // namespace Graphics
} // namespace Lumino

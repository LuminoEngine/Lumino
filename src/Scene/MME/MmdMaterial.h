
#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Scene/Material.h>

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class MmdMaterial
	: public Material2
{
public:
	void SetToonTexture(Texture* texture);
	Texture* GetToonTexture(Texture* texture) const { return m_toonTexture; }
	void SetSphereTexture(Texture* texture);
	Texture* GetSphereTexture(Texture* texture) const { return m_sphereTexture; }
	
LN_INTERNAL_ACCESS:
	MmdMaterial();
	virtual ~MmdMaterial();

public:	// TODO:
	ColorF		m_diffuse;			// 物体の色
	ColorF		m_ambient;			// 環境光
	ColorF		m_specular;			// 光沢
	ColorF		m_emissive;			// 物体の発光色 ( 光源の影響を受けない色 )
	float		m_power;			// 光沢の強さ

	Texture*	m_toonTexture;
	Texture*	m_sphereTexture;

	ColorF						ToonColor;			///< [PMD] トゥーンカラー
	ColorF						EdgeColor;			///< [PMX] エッジカラー
	float						EdgeSize;			///< [PMX] エッジサイズ
	ColorF						TextureCoe;			///< [PMX] テクスチャ係数
	ColorF						SphereTextureCoe;	///< [PMX] スフィアテクスチャ係数
	ColorF						ToonTextureCoe;		///< [PMX] Toonテクスチャ係数
	uint32_t					DrawingFlags;		///< [PMX] 描画オプション (MMDDrawingFlags の組み合わせ)
	int/*Material::SphereMode*/		SphereMode;			///< [PMX] スフィアテクスチャの合成モード
};

LN_NAMESPACE_END

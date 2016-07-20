
#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Scene/Material.h>

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class MmdMaterialInstance
	: public detail::MaterialInstance
{
public:
	//void SetToonTexture(Texture* texture);
	//Texture* GetToonTexture() const { return m_toonTexture; }
	//void SetSphereTexture(Texture* texture);
	//Texture* GetSphereTexture() const { return m_sphereTexture; }

	virtual void OnCombine(Material3* owner, Material3* parent) override;

LN_INTERNAL_ACCESS:
	MmdMaterialInstance();
	virtual ~MmdMaterialInstance();

public:	// TODO:
	Color		m_diffuse;			// 物体の色
	Color		m_ambient;			// 環境光
	Color		m_specular;			// 光沢
	Color		m_emissive;			// 物体の発光色 ( 光源の影響を受けない色 )
	float		m_power;			// 光沢の強さ

	RefPtr<Texture>	m_toonTexture;
	RefPtr<Texture>	m_sphereTexture;

	Color						ToonColor;			///< [PMD] トゥーンカラー
	Color						EdgeColor;			///< [PMX] エッジカラー
	float						EdgeSize;			///< [PMX] エッジサイズ
	Color						TextureCoe;			///< [PMX] テクスチャ係数
	Color						SphereTextureCoe;	///< [PMX] スフィアテクスチャ係数
	Color						ToonTextureCoe;		///< [PMX] Toonテクスチャ係数
	uint32_t					DrawingFlags;		///< [PMX] 描画オプション (MMDDrawingFlags の組み合わせ)
	int/*Material::SphereMode*/		SphereMode;			///< [PMX] スフィアテクスチャの合成モード
};

LN_NAMESPACE_END


#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Scene/VisualNode.h>

LN_NAMESPACE_BEGIN

/** 描画オプション */
LN_ENUM_FLAGS(MmdDrawingFlags)
{
	None			= 0x0000,		/**< 指定無し */
	CullingDouble	= 0x0001,		/**< 両面描画 */
	GroundShadow	= 0x0002,		/**< 地面影 */
	SelfShadowMap	= 0x0004,		/**< セルフシャドウマップへの描画 */
	SelfShadow		= 0x0008,		/**< セルフシャドウの描画 */
	Edge			= 0x0010,		/**< エッジ描画 */
};
LN_ENUM_FLAGS_DECLARE(MmdDrawingFlags);

/** スフィアテクスチャの合成モード */
enum class MmdSphereMode
{
	Disable			= 0,			/**< 無効 */
	Mul				= 1,			/**< 乗算(sph) */
	Add				= 2,			/**< 加算(spa) */
	SubTexture		= 3,			/**< サブテクスチャ(追加UV1のx,yをUV参照して通常テクスチャ描画を行う) */
};

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

	virtual void OnCombine(Material* owner, Material* parent) override;

LN_INTERNAL_ACCESS:
	MmdMaterialInstance();
	virtual ~MmdMaterialInstance();

public:	// TODO:
	Color		m_diffuse;			// 物体の色
	Color		m_ambient;			// 環境光
	Color		m_specular;			// 光沢
	Color		m_emissive;			// 物体の発光色 ( 光源の影響を受けない色 )
	float		m_power;			// 光沢の強さ

	RefPtr<Texture>	m_materialTexture;
	RefPtr<Texture>	m_toonTexture;
	RefPtr<Texture>	m_sphereTexture;

	Color						ToonColor;			///< [PMD] トゥーンカラー
	Color						EdgeColor;			///< [PMX] エッジカラー
	float						EdgeSize;			///< [PMX] エッジサイズ
	Color						TextureCoe;			///< [PMX] テクスチャ係数
	Color						SphereTextureCoe;	///< [PMX] スフィアテクスチャ係数
	Color						ToonTextureCoe;		///< [PMX] Toonテクスチャ係数
	uint32_t					DrawingFlags;		///< [PMX] 描画オプション (MMDDrawingFlags の組み合わせ)
	MmdSphereMode				SphereMode;			///< [PMX] スフィアテクスチャの合成モード
};

LN_NAMESPACE_END

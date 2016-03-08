
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief
*/
class SpriteBase
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	//
	///**
	//	@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
	//*/
	//static SpriteBase* Create();

	///**
	//	@brief		
	//*/
	//static SpriteBase* Create3D();

public:

	/// 作成
	//void Create(/*SpriteCoord spriteCoord = SpriteCoord_RX, SceneGraphManager* manager = NULL*/);

	/// テクスチャの設定
	void SetTexture(Texture* texture);

	/// テクスチャの取得
	Texture* GetTexture() const;

	/// サイズの設定
	void SetSize(const SizeF& size) { m_size = size; UpdateVertexData(); }

	/// テクスチャの転送矩形の設定 (ピクセル単位)
	void SetSrcRect(const Rect& rect);

	/// テクスチャの転送矩形の取得 (ピクセル単位)
	const Rect& GetSrcRect() const { return m_srcRect; }

	/// 反転方法の設定
	void SetFlipMode(FlipMode mode) { m_flipMode = mode; UpdateTexUV(); }

	/// 反転方法の取得
	FlipMode GetFlipMode() const { return m_flipMode; }

private:
	void UpdateTexUV();
	void UpdateVertexData();

	/// ピクセル単位の転送矩形をテクスチャUVの矩形 (0.0～1.0) に変換する
	static void NormalizeSrcRect(const Rect& srcRect, const Size& textureSize, float* l, float* t, float* r, float* b);

protected:
	SpriteBase();
	virtual ~SpriteBase();
	void Initialize(SceneGraphManager* manager, SpriteCoord spriteCoord);

	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	SpriteCoord	m_spriteCoord;
	SizeF		m_size;
	Rect		m_srcRect;
	FlipMode	m_flipMode;

	Vector3		m_upperLeft;
	Vector3		m_lowerRight;
	Vector2		m_upperLeftUV;
	Vector2		m_lowerRightUV;

	friend class Internal::SceneHelper;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

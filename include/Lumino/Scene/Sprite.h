
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/**
	@brief
*/
class Sprite
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
	void SetSize(const Size& size) { m_size = size; UpdateVertexData(); }


	/// テクスチャの転送矩形の設定 (ピクセル単位)
	void SetSrcRect(const Rect& rect);
	void SetSrcRect(int x, int y, int width, int height) { SetSrcRect(Rect(x, y, width, height)); }

	/// テクスチャの転送矩形の取得 (ピクセル単位)
	const Rect& GetSrcRect() const { return m_srcRect; }

	/// 反転方法の設定
	void SetFlipMode(FlipMode mode) { m_flipMode = mode; UpdateTexUV(); }

	/// 反転方法の取得
	FlipMode GetFlipMode() const { return m_flipMode; }

private:

	/// ピクセル単位の転送矩形をテクスチャUVの矩形 (0.0～1.0) に変換する
	static void NormalizeSrcRect(const Rect& srcRect, const SizeI& textureSize, float* l, float* t, float* r, float* b);

protected:
	Sprite();
	virtual ~Sprite();
	void Initialize(SceneGraph* owner, SpriteCoord spriteCoord);

	void UpdateTexUV();
	virtual void UpdateVertexData();
	//virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;


protected:
	SpriteCoord	m_spriteCoord;
	Size		m_size;
	Rect		m_srcRect;
	FlipMode	m_flipMode;

	Vector3		m_upperLeft;
	Vector3		m_lowerRight;
	Vector2		m_upperLeftUV;
	Vector2		m_lowerRightUV;

	Size		m_renderSize;
	Rect		m_renderSourceRect;
};

/**
	@brief	2D 空間に配置されるスプライトのクラスです。
*/
class Sprite2D
	: public Sprite
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
	*/
	static Sprite2DPtr Create();
	
	/**
		@brief		指定したファイルパスのテクスチャを読み込み、スプライトを作成します。
	*/
	static Sprite2DPtr Create(const StringRef& filePath);

	static Sprite2DPtr Create(Texture* texture);

public:

	/** 
		@brief		スプライトの原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。
					(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
	*/
	void SetAnchorPoint(const Vector2& ratio);
	void SetAnchorPoint(float ratioX, float ratioY);		/**< @overload SetAnchorPoint */

LN_PROTECTED_INTERNAL_ACCESS:
	Sprite2D();
	virtual ~Sprite2D();
	void Initialize(SceneGraph* owner);
	//virtual void OnRender(SceneGraphRenderingContext* dc) override;

	virtual void OnRender2(DrawList* renderer) override;

private:
	Vector2	m_anchor;
};

/**
	@brief	3D 空間に配置されるスプライトのクラスです。
*/
class Sprite3D
	: public Sprite
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
	*/
	static Sprite3DPtr Create();

	static Sprite3DPtr Create(float width, float height);
	static Sprite3DPtr Create(float width, float height, Texture* texture);

protected:
	Sprite3D();
	virtual ~Sprite3D();
	void Initialize(SceneGraph* owner);
	virtual detail::Sphere GetBoundingSphere() override;
	//virtual void OnRender(SceneGraphRenderingContext* dc) override;
	//virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;
	virtual void UpdateVertexData() override;

private:
	detail::Sphere	m_boundingSphere;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

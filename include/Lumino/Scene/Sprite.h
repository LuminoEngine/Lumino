
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

	/**< スプライトが表示するテクスチャを設定します。*/
	void SetTexture(Texture* texture);

	/**< スプライトが表示するテクスチャを取得します。*/
	Texture* GetTexture() const;

	/**< スプライトの大きさを設定します。*/
	void SetSize(const Size& size);

	/**< テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) */
	void SetTextureRect(const RectF& rect);
	void SetTextureRect(float x, float y, float width, float height);

	/// 反転方法の設定
	//void SetFlipMode(FlipMode mode) { m_flipMode = mode; UpdateTexUV(); }

	/// 反転方法の取得
	//FlipMode GetFlipMode() const { return m_flipMode; }

LN_PROTECTED_INTERNAL_ACCESS:
	Sprite();
	virtual ~Sprite();
	void Initialize(SceneGraph* owner, SpriteCoord spriteCoord);
	void UpdateVertexData();

	SpriteCoord	m_spriteCoord;
	Size		m_size;
	RectF		m_srcRect;
	FlipMode	m_flipMode;
	Size		m_renderSize;
	RectF		m_renderSourceRect;
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
	virtual void OnRender2(DrawList* renderer) override;

private:
	Vector2	m_anchor;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

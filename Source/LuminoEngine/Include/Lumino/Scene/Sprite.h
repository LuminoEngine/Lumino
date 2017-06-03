
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class Sprite3DComponent;
typedef RefPtr<Sprite3DComponent>	Sprite3DComponentPtr;
class Sprite2DComponent;
typedef RefPtr<Sprite2DComponent>	Sprite2DComponentPtr;

/**
	@brief		スプライトの抽象クラスです。スプライトオブジェクトを作成するには Sprite2D または Sprite3D クラス使用します。
*/
LN_CLASS()
class SpriteComponent
	: public VisualComponent
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		スプライトが表示するテクスチャを設定します。
	*/
	LN_METHOD(Property)
	void SetTexture(Texture* texture);
	
	/**
		@brief		スプライトが表示するテクスチャを取得します。
	*/
	LN_METHOD(Property)
	Texture* GetTexture() const;

	/**
		@brief		スプライトの大きさを設定します。
	*/
	void SetSize(const Size& size);

	/**
		@brief		テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位)
	*/
	void SetSourceRect(const Rect& rect);
	void SetSourceRect(float x, float y, float width, float height);	/**< @overload SetSourceRect */

	const Rect& GetSourceRect() const { return m_srcRect; }

	/** 
		@brief		スプライトの原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。
					(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
	*/
	void SetAnchorPoint(const Vector2& ratio);
	void SetAnchorPoint(float ratioX, float ratioY);					/**< @overload SetAnchorPoint */


	/// 反転方法の設定
	//void SetFlipMode(FlipMode mode) { m_flipMode = mode; UpdateTexUV(); }

	/// 反転方法の取得
	//FlipMode GetFlipMode() const { return m_flipMode; }

LN_PROTECTED_INTERNAL_ACCESS:
	SpriteComponent();
	virtual ~SpriteComponent();
	void Initialize();
	void RenderSprite(DrawList* renderer, SpriteBaseDirection dir);

private:
	void UpdateVertexData();

	Size				m_size;
	Rect				m_srcRect;
	Vector2				m_anchor;
	FlipMode			m_flipMode;
	RefPtr<Texture>		m_texture;
	RefPtr<Material>	m_material;
	Size				m_renderSize;
	Rect				m_renderSourceRect;
};

/**
	@brief	2D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite2DComponent
	: public SpriteComponent
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
	*/
	static Sprite2DComponentPtr Create();
	
	/**
		@brief		指定したファイルパスのテクスチャを読み込み、スプライトを作成します。
		@param[in]	filePath	: 画像ファイルのファイルパス
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、読み込んだテクスチャのサイズとなります。
	*/
	static Sprite2DComponentPtr Create(const StringRef& filePath);
	
	/**
		@brief		テクスチャを指定してスプライトを作成します。
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、指定したテクスチャのサイズとなります。
	*/
	static Sprite2DComponentPtr Create(Texture* texture);

LN_PROTECTED_INTERNAL_ACCESS:
	Sprite2DComponent();
	virtual ~Sprite2DComponent();
	LN_METHOD()
	void Initialize();
	LN_METHOD()
	void Initialize(Texture* texture);
	virtual void OnRender2(DrawList* renderer) override;
};

/**
	@brief	3D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite3DComponent
	: public SpriteComponent
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Sprite3DComponentPtr Create();
	
	
	/**
		@brief		サイズを指定して、テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Sprite3DComponentPtr Create(float width, float height);
	
	/**
		@brief		サイズとテクスチャを指定してスプライトを作成します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Sprite3DComponentPtr Create(float width, float height, Texture* texture);

LN_PROTECTED_INTERNAL_ACCESS:
	Sprite3DComponent();
	virtual ~Sprite3DComponent();
	void Initialize();
	virtual void OnRender2(DrawList* renderer) override;
};


/**
	@brief	スプライトオブジェクトのベースクラスです。
*/
LN_CLASS()
class SpriteBase
	: public VisualObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/** スプライトが表示するテクスチャを設定します。 */
	LN_METHOD(Property)
	void SetTexture(Texture* texture);
	
	/** スプライトが表示するテクスチャを取得します。 */
	LN_METHOD(Property)
	Texture* GetTexture() const;

	/** テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。 */
	LN_METHOD(Property)
	void SetSourceRect(const Rect& rect);

	/** @overload SetSourceRect */
	LN_METHOD()
	void SetSourceRect(float x, float y, float width, float height);

	/** テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位) */
	LN_METHOD(Property)
	const Rect& GetSourceRect() const;
	
	/** 
		@brief		スプライトの原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。
					(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
	*/
	LN_METHOD(Property)
	void SetAnchorPoint(const Vector2& ratio);

	/** @overload SetSourceRect */
	LN_METHOD()
	void SetAnchorPoint(float ratioX, float ratioY);

protected:
	virtual SpriteComponent* GetSpriteComponent() const = 0;

LN_CONSTRUCT_ACCESS:
	SpriteBase();
	virtual ~SpriteBase();

	LN_METHOD()
	void Initialize();
};

/**
	@brief	2D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite2D
	: public SpriteBase
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
	*/
	static RefPtr<Sprite2D> Create();
	
	/**
		@brief		指定したファイルパスのテクスチャを読み込み、スプライトを作成します。
		@param[in]	filePath	: 画像ファイルのファイルパス
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、読み込んだテクスチャのサイズとなります。
	*/
	static RefPtr<Sprite2D> Create(const StringRef& filePath);
	
	/**
		@brief		テクスチャを指定してスプライトを作成します。
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、指定したテクスチャのサイズとなります。
	*/
	static RefPtr<Sprite2D> Create(Texture* texture);

protected:
	virtual VisualComponent* GetMainVisualComponent() const override;
	virtual SpriteComponent* GetSpriteComponent() const override;

LN_CONSTRUCT_ACCESS:
	Sprite2D();
	virtual ~Sprite2D();

	LN_METHOD()
	void Initialize();

	LN_METHOD()
	void Initialize(const StringRef& filePath);

	LN_METHOD()
	void Initialize(Texture* texture);

private:
	RefPtr<Sprite2DComponent>	m_component;
};

/**
	@brief	3D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite3D
	: public SpriteBase
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static RefPtr<Sprite3D> Create();
	
	
	/**
		@brief		サイズを指定して、テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには SetTexture() を使用します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	//static RefPtr<Sprite3D> Create(float width, float height);
	
	/**
		@brief		サイズとテクスチャを指定してスプライトを作成します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static RefPtr<Sprite3D> Create(float width, float height, Texture* texture);


protected:
	virtual VisualComponent* GetMainVisualComponent() const override;
	virtual SpriteComponent* GetSpriteComponent() const override;

LN_CONSTRUCT_ACCESS:
	Sprite3D();
	virtual ~Sprite3D();

	LN_METHOD()
	void Initialize();

	LN_METHOD()
	void Initialize(float width, float height, Texture* texture);

private:
	RefPtr<Sprite3DComponent>	m_component;
};


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

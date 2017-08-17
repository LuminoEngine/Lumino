
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class Sprite3DComponent;
typedef Ref<Sprite3DComponent>	Sprite3DComponentPtr;
class Sprite2DComponent;
typedef Ref<Sprite2DComponent>	Sprite2DComponentPtr;

/**
	@brief		スプライトの抽象クラスです。スプライトオブジェクトを作成するには Sprite2D または Sprite3D クラス使用します。
*/
LN_CLASS()
class SpriteComponent
	: public VisualComponent
{
	LN_OBJECT;
public:

	/**
		@brief		スプライトが表示するテクスチャを設定します。
	*/
	LN_METHOD(Property)
	void setTexture(Texture* texture);
	
	/**
		@brief		スプライトが表示するテクスチャを取得します。
	*/
	LN_METHOD(Property)
	Texture* getTexture() const;

	/**
		@brief		スプライトの大きさを設定します。
	*/
	void setSize(const Size& size);

	/**
		@brief		テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位)
	*/
	void setSourceRect(const Rect& rect);
	void setSourceRect(float x, float y, float width, float height);	/**< @overload setSourceRect */

	const Rect& getSourceRect() const { return m_srcRect; }

	/** 
		@brief		スプライトの原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。
					(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
	*/
	void setAnchorPoint(const Vector2& ratio);
	void setAnchorPoint(float ratioX, float ratioY);					/**< @overload setAnchorPoint */


	/// 反転方法の設定
	//void SetFlipMode(FlipMode mode) { m_flipMode = mode; UpdateTexUV(); }

	/// 反転方法の取得
	//FlipMode GetFlipMode() const { return m_flipMode; }

LN_PROTECTED_INTERNAL_ACCESS:
	SpriteComponent();
	virtual ~SpriteComponent();
	void initialize();
	void renderSprite(RenderingContext* renderer, SpriteBaseDirection dir);

private:
	void updateVertexData();

	Size				m_size;
	Rect				m_srcRect;
	Vector2				m_anchor;
	FlipMode			m_flipMode;
	Ref<Texture>		m_texture;
	Ref<Material>	m_material;
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
	LN_OBJECT;
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには setTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
	*/
	static Sprite2DComponentPtr create();
	
	/**
		@brief		指定したファイルパスのテクスチャをアセットから読み込み、スプライトを作成します。
		@param[in]	filePath	: 画像ファイルのファイルパス
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、読み込んだテクスチャのサイズとなります。
	*/
	static Sprite2DComponentPtr create(const StringRef& filePath);
	
	/**
		@brief		テクスチャを指定してスプライトを作成します。
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、指定したテクスチャのサイズとなります。
	*/
	static Sprite2DComponentPtr create(Texture* texture);

LN_PROTECTED_INTERNAL_ACCESS:
	Sprite2DComponent();
	virtual ~Sprite2DComponent();
	LN_METHOD()
	void initialize();
	LN_METHOD()
	void initialize(Texture* texture);
	virtual void onRender2(RenderingContext* renderer) override;
};

/**
	@brief	3D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite3DComponent
	: public SpriteComponent
{
	LN_OBJECT;
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには setTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Sprite3DComponentPtr create();
	
	
	/**
		@brief		サイズを指定して、テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには setTexture() を使用します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Sprite3DComponentPtr create(float width, float height);
	
	/**
		@brief		サイズとテクスチャを指定してスプライトを作成します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Sprite3DComponentPtr create(float width, float height, Texture* texture);

LN_PROTECTED_INTERNAL_ACCESS:
	Sprite3DComponent();
	virtual ~Sprite3DComponent();
	void initialize();
	virtual void onRender2(RenderingContext* renderer) override;
};


/**
	@brief	スプライトオブジェクトのベースクラスです。
*/
LN_CLASS()
class SpriteBase
	: public VisualObject
{
	LN_OBJECT;
public:
	
	/** スプライトが表示するテクスチャを設定します。 */
	LN_METHOD(Property)
	void setTexture(Texture* texture);
	
	/** スプライトが表示するテクスチャを取得します。 */
	LN_METHOD(Property)
	Texture* getTexture() const;

	/** テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。 */
	LN_METHOD(Property)
	void setSourceRect(const Rect& rect);

	/** @overload setSourceRect */
	LN_METHOD()
	void setSourceRect(float x, float y, float width, float height);

	/** テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位) */
	LN_METHOD(Property)
	const Rect& getSourceRect() const;
	
	/** 
		@brief		スプライトの原点を設定します。
		@details	値は (0,0) から (1,1) の間で指定します。
					デフォルトは (0,0) で、これはスプライトの左上が原点であることを意味します。
					(0.5,0.5) はスプライトの中央、(1,1) は右下が原点となります。
	*/
	LN_METHOD(Property)
	void setAnchorPoint(const Vector2& ratio);

	/** @overload setSourceRect */
	LN_METHOD()
	void setAnchorPoint(float ratioX, float ratioY);

protected:
	virtual SpriteComponent* getSpriteComponent() const = 0;

LN_CONSTRUCT_ACCESS:
	SpriteBase();
	virtual ~SpriteBase();

	LN_METHOD()
	void initialize();
};

/**
	@brief	2D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite2D
	: public SpriteBase
{
	LN_OBJECT;
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには setTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
	*/
	static Ref<Sprite2D> create();
	
	/**
		@brief		指定したファイルパスのテクスチャを読み込み、スプライトを作成します。
		@param[in]	filePath	: 画像ファイルのファイルパス
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、読み込んだテクスチャのサイズとなります。
	*/
	static Ref<Sprite2D> create(const StringRef& filePath);
	
	/**
		@brief		テクスチャを指定してスプライトを作成します。
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。
					スプライトのサイズは、指定したテクスチャのサイズとなります。
	*/
	static Ref<Sprite2D> create(Texture* texture);

protected:
	virtual VisualComponent* getMainVisualComponent() const override;
	virtual SpriteComponent* getSpriteComponent() const override;

LN_CONSTRUCT_ACCESS:
	Sprite2D();
	virtual ~Sprite2D();

	LN_METHOD()
	void initialize();

	LN_METHOD()
	void initialize(const StringRef& filePath);

	LN_METHOD()
	void initialize(Texture* texture);

private:
	Ref<Sprite2DComponent>	m_component;
};

/**
	@brief	3D 空間に配置されるスプライトのクラスです。
*/
LN_CLASS()
class Sprite3D
	: public SpriteBase
{
	LN_OBJECT;
public:
	
	/**
		@brief		テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには setTexture() を使用します。
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Ref<Sprite3D> create();
	
	
	/**
		@brief		サイズを指定して、テクスチャを持たないスプライトを作成します。テクスチャを割り当てるには setTexture() を使用します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	//static Ref<Sprite3D> create(float width, float height);
	
	/**
		@brief		サイズとテクスチャを指定してスプライトを作成します。
		@param[in]	width		: スプライトの幅
		@param[in]	height		: スプライトの高さ
		@param[in]	texture		: スプライトが表示するテクスチャ
		@detail		作成されたスプライトは、デフォルトの 3D シーングラフ に追加されます。
	*/
	static Ref<Sprite3D> create(float width, float height, Texture* texture);


	virtual SpriteComponent* getSpriteComponent() const override;

public:
	virtual VisualComponent* getMainVisualComponent() const override;

LN_CONSTRUCT_ACCESS:
	Sprite3D();
	virtual ~Sprite3D();

	LN_METHOD()
	void initialize();

	LN_METHOD()
	void initialize(float width, float height, Texture* texture);

private:
	Ref<Sprite3DComponent>	m_component;
};


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

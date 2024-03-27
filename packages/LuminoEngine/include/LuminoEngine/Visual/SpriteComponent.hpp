
#pragma once
#include "VisualComponent.hpp"

namespace ln {
class World;
class Component;

class SpriteFrame
	: public Object
{
	LN_OBJECT;
public:
	/** テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) */
	void setSourceRect(const Rect& value) { m_sourceRect = value; }

	/** テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) */
	const Rect& sourceRect() const { return m_sourceRect; }

	/** フレームのアンカーポイントを設定します。デフォルトは nan で、これはアンカーポイントが定義されていないことを示します。 */
	void setAnchorPoint(const Vector2& value) { m_anchorPoint = value; }

	/** フレームのアンカーポイントを取得します。 */
	const Vector2& anchorPoint() const { return m_anchorPoint; }

	/** フレームにアンカーポイントが定義されているかどうかを確認します。 */
	bool hasAnchorPoint() const { return Math::isNaN(m_anchorPoint.x) || Math::isNaN(m_anchorPoint.y); }

protected:
	void serialize_deprecated(Serializer2_deprecated& ar) override;

LN_CONSTRUCT_ACCESS:
	SpriteFrame();
	virtual ~SpriteFrame() = default;
	void init();

private:
	Rect m_sourceRect;
	Vector2 m_anchorPoint;
	//Ref<Texture> m_texture;	// TODO
};

// TODO: Effect でも使う。Material はこっちに持たせた方がいいかも
class SpriteSheet
	: public Object
{
	// テクスチャの優先順位は SpriteComponent > SpriteSheet > SpriteFrame
	// 例えば SpriteSheet はメモリ削減のため共有しつつ、テクスチャだけ変えたい場合、
	// 複数の SpriteComponent には同じ SpriteSheet をセットし、SpriteComponent::setTexture で異なるテクスチャをセットする。
	LN_OBJECT;
public:
	// frames, animation


	static Ref<SpriteSheet> load(StringView path);
	static Ref<SpriteSheet> create(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint = Vector2::NaN);

    Texture* texture() const;

public: // TODO: internal
	int frameCount() const { return m_frames.size(); }
	SpriteFrame* frame(int index) const;

protected:
	void serialize_deprecated(Serializer2_deprecated& ar) override;

LN_CONSTRUCT_ACCESS:
	SpriteSheet();
	virtual ~SpriteSheet() = default;
	void init();
	void init(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint = Vector2::NaN);

private:
    void clear();
    void splitFrames();

    int m_frameWidth;
    int m_frameHeight;
    Vector2 m_anchorPoint;
	Ref<Texture> m_texture;
	List<Ref<SpriteFrame>> m_frames;
};


LN_CLASS()
class SpriteComponent
	: public VisualComponent
{
	LN_OBJECT;
public:
    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);

    Texture* texture() const;

    /** スプライトの大きさを設定します。 */
    void setSize(const Size& size);

    /** アンカーポイント (原点) を設定します。デフォルトは中心 (0.5, 0.5) です。 */
    void setAnchorPoint(const Vector2& value) { m_anchorPoint = value; }
	void setAnchorPoint(float u, float v) { setAnchorPoint(Vector2(u, v)); }

    /** アンカーポイント (原点) を取得します。 */
    const Vector2& anchorPoint() const { return m_anchorPoint; }


    /**
        @brief		テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位)
    */
    void setSourceRect(const Rect& rect);

    const Rect& sourceRect() const { return m_sourceRect; }

	/** SpriteSheet を設定します。 */
	void setFrameSet(SpriteSheet* value);

	/** SpriteSheet から適用するフレームインデックスを設定します。デフォルトは -1 で、これはフレームを使用しないことを示します。 */
	void setFrameIndex(int index);
	int frameIndex() const { return m_frameIndex; }

	SpriteSheet* spriteSheet() const;


    /** 表示するテクスチャを水平に反転するかどうかを設定します。 */
    void setFlippedX(bool value) { m_flipFlags.set(SpriteFlipFlags::FlipX, value); }

    /** 表示するテクスチャを水平に反転するかどうかを確認します。 */
    bool isFlippedX() const { return m_flipFlags.hasFlag(SpriteFlipFlags::FlipX); }

    /** 表示するテクスチャを垂直に反転するかどうかを設定します。 */
    void setFlippedY(bool value) { m_flipFlags.set(SpriteFlipFlags::FlipY, value); }

    /** 表示するテクスチャを垂直に反転するかどうかを確認します。 */
    bool isFlippedY() const { return m_flipFlags.hasFlag(SpriteFlipFlags::FlipY); }

    void setPixelsParUnit(float value) { m_pixelsParUnit = value; }

	// TODO: internal
	//static void registerType(EngineContext* context);

	void setBillboardType(BillboardType value) { m_billboardType = value; }

protected:
    void onRender(RenderingContext* context) override;

	void serialize_deprecated(Serializer2_deprecated& ar) override;

LN_CONSTRUCT_ACCESS:
    SpriteComponent();
	virtual ~SpriteComponent();
	bool init();


private:
    Ref<Material> m_material;
    Size m_size;
    Rect m_sourceRect;
    Vector2 m_anchorPoint;
	Ref<SpriteSheet> m_frameSet;
	int m_frameIndex;
    Flags<SpriteFlipFlags> m_flipFlags;
    float m_pixelsParUnit;
	BillboardType m_billboardType;
};

} // namespace ln


#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "VisualComponent.hpp"

namespace ln {
class Material;
class World;
class Component;

class SpriteFrame
	: public Object
{
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

LN_CONSTRUCT_ACCESS:
	SpriteFrame();
	virtual ~SpriteFrame() = default;
	void init();

private:
	Rect m_sourceRect;
	Vector2 m_anchorPoint;
	//Ref<Texture> m_texture;	// TODO
};


class SpriteFrameSet
	: public Object
{
	// テクスチャの優先順位は SpriteComponent > SpriteFrameSet > SpriteFrame
	// 例えば SpriteFrameSet はメモリ削減のため共有しつつ、テクスチャだけ変えたい場合、
	// 複数の SpriteComponent には同じ SpriteFrameSet をセットし、SpriteComponent::setTexture で異なるテクスチャをセットする。
public:
	// frames, animation


	static Ref<SpriteFrameSet> create(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint = Vector2::NaN);

    Texture* texture() const;

public: // TODO: internal
	int frameCount() const { return m_frames->size(); }
	SpriteFrame* frame(int index) const;

LN_CONSTRUCT_ACCESS:
	SpriteFrameSet();
	virtual ~SpriteFrameSet() = default;
	void init();
	void init(Texture* texture, int frameWidth, int frameHeight, const Vector2& anchorPoint = Vector2::NaN);

private:
	Ref<Texture> m_texture;
	Ref<List<Ref<SpriteFrame>>> m_frames;
};


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

    /** アンカーポイント (原点) を取得します。 */
    const Vector2& anchorPoint() const { return m_anchorPoint; }


    /**
        @brief		テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位)
    */
    void setSourceRect(const Rect& rect);

    const Rect& sourceRect() const { return m_sourceRect; }

	/** SpriteFrameSet を設定します。 */
	void setFrameSet(SpriteFrameSet* value);

	/** SpriteFrameSet から適用するフレームインデックスを設定します。デフォルトは -1 で、これはフレームを使用しないことを示します。 */
	void setFrameIndex(int index);
	int frameIndex() const { return m_frameIndex; }


	// TODO: internal
	static void registerType(EngineContext* context);

protected:
    void onRender(RenderingContext* context);


LN_CONSTRUCT_ACCESS:
    SpriteComponent();
	virtual ~SpriteComponent();
	void init();


private:
    Ref<Material> m_material;
    Size m_size;
    Rect m_sourceRect;
    Vector2 m_anchorPoint;
	Ref<SpriteFrameSet> m_frameSet;
	int m_frameIndex;
};

} // namespace ln

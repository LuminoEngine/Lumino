
#pragma once
#include "VisualObject.hpp"

namespace ln {
class Texture;
class SpriteFrameSet;
class SpriteComponent;

/**
 * スプライトオブジェクトを表します。
 *
 * デフォルトの BlendMode は Alpha です。
 * デフォルトの CullMode は None です。（両面表示となります）
 */
LN_CLASS()
class Sprite
	: public VisualObject
{
	LN_OBJECT;
public:
    static Ref<Sprite> create(Texture* texture, float width, float height);
	static Ref<Sprite> create(SpriteFrameSet* frameSet);

    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);

    /** スプライトの大きさを設定します。 */
    void setSize(const Size& size);

	/** テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。 */
	//LN_METHOD(Property)
	void setSourceRect(const Rect& rect);

	/** @overload setSourceRect */
	LN_METHOD(OverloadPostfix = "XYWH")
	void setSourceRect(float x, float y, float width, float height);

	/** テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位) */
	//LN_METHOD(Property)
	const Rect& sourceRect() const;

	/** SpriteFrameSet を設定します。 */
	void setFrameSet(SpriteFrameSet* value);

	/** SpriteFrameSet から適用するフレームインデックスを設定します。デフォルトは -1 で、これはフレームを使用しないことを示します。 */
	void setFrameIndex(int index);



    /**
        @brief		スプライトの原点を設定します。
        @details	値は (0,0) から (1,1) の間で指定します。
                    デフォルトは (0,0) で、これはスプライトの左下が原点であることを意味します。
                    (0.5,0.5) はスプライトの中央、(1,1) は右上が原点となります。
    */


    /** アンカーポイント (原点) を設定します。デフォルトは中心(0.5, 0.5) です。 */
    void setAnchorPoint(const Vector2& value);

    /** アンカーポイント (原点) を取得します。 */
    const Vector2& anchorPoint() const;

    /** 表示するテクスチャを水平に反転するかどうかを設定します。 */
    void setFlippedX(bool value);

    /** 表示するテクスチャを水平に反転するかどうかを確認します。 */
    bool isFlippedX() const;

    /** 表示するテクスチャを垂直に反転するかどうかを設定します。 */
    void setFlippedY(bool value);

    /** 表示するテクスチャを垂直に反転するかどうかを確認します。 */
    bool isFlippedY() const;

protected:

LN_CONSTRUCT_ACCESS:
	Sprite();
	virtual ~Sprite();

	void init();

	/** init */
	LN_METHOD()
    void init(Texture* texture, float width, float height);

	void init(SpriteFrameSet* frameSet);

private:
    Ref<SpriteComponent> m_component;
};

} // namespace ln

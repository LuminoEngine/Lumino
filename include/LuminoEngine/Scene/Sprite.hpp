
#pragma once
#include "VisualObject.hpp"

namespace ln {
class Texture;
class SpriteComponent;

class Sprite
	: public VisualObject
{
public:
    static Ref<Sprite> create(float width, float height, Texture* texture = nullptr);

    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);

    /** スプライトの大きさを設定します。 */
    void setSize(const Size& size);


    /**
        @brief		テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位)
    */
    void setSourceRect(const Rect& rect);
    void setSourceRect(float x, float y, float width, float height);	/**< @overload setSourceRect */

    const Rect& sourceRect() const;

protected:

LN_CONSTRUCT_ACCESS:
	Sprite();
	virtual ~Sprite();
	void initialize();
    void initialize(float width, float height, Texture* texture = nullptr);

private:
    Ref<SpriteComponent> m_component;
};

} // namespace ln

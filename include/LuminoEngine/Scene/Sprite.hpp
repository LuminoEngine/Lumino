
#pragma once
#include "WorldObject.hpp"

namespace ln {
class Texture;
class SpriteComponent;

class Sprite
	: public WorldObject
{
public:
    static Ref<Sprite> create(float width, float height);

    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);

    /** スプライトの大きさを設定します。 */
    void setSize(const Size& size);

protected:

LN_CONSTRUCT_ACCESS:
	Sprite();
	virtual ~Sprite();
	void initialize();
    void initialize(float width, float height);

private:
    Ref<SpriteComponent> m_component;
};

} // namespace ln

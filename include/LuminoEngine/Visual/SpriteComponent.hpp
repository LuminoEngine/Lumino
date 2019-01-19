
#pragma once
#include "../Graphics/GeometryStructs.hpp"
#include "VisualComponent.hpp"

namespace ln {
class Material;
class World;
class Component;

class SpriteComponent
	: public VisualComponent
{
public:
    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);

    Texture* texture() const;

    /** スプライトの大きさを設定します。 */
    void setSize(const Size& size);


    /**
        @brief		テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位)
    */
    void setSourceRect(const Rect& rect);

    const Rect& sourceRect() const { return m_sourceRect; }

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
};

} // namespace ln

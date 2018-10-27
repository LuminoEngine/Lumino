
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

    /** スプライトの大きさを設定します。 */
    void setSize(const Size& size);

protected:
    void onRender(RenderingContext* context);


LN_CONSTRUCT_ACCESS:
    SpriteComponent();
	virtual ~SpriteComponent();
	void initialize();

private:
    Ref<Material> m_material;
};

} // namespace ln

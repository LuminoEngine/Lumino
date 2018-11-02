#pragma once
#include "UIElement.hpp"

namespace ln {
class Material;

class UISprite
    : public UIElement
{
public:
    /** スプライトが表示するテクスチャを設定します。 */
    LN_METHOD(Property)
    void setTexture(Texture* texture);

    UISprite();
	void initialize();

protected:
    virtual void onRender(UIRenderingContext* context);

private:
    Ref<Material> m_material;
};

} // namespace ln


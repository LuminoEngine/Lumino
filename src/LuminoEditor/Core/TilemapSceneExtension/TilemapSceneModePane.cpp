
#include "TilemapSceneModePane.hpp"

namespace lna {

//==============================================================================
// TilemapSceneModePane

void TilemapSceneModePane::init()
{
    EditorPane::init();
    m_material = ln::makeObject<ln::Material>();
}

void TilemapSceneModePane::setTileset(ln::Tileset* tileset)
{
    m_tileset = tileset;

    auto texture = m_tileset->material()->mainTexture();
    if (texture) {
        m_material->setMainTexture(texture);
        setWidth(texture->width());
        setHeight(texture->height());
    }
}

void TilemapSceneModePane::onRender(ln::UIRenderingContext* context)
{
    if (m_material && m_material->mainTexture()) {
        context->drawImage(ln::Rect(0, 0, m_material->mainTexture()->width(), m_material->mainTexture()->height()), m_material);
    }
}

} // namespace lna

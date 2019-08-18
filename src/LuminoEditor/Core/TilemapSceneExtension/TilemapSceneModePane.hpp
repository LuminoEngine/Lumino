#pragma once
#include "../App/ToolPanesArea.hpp"

namespace lna {

class TilemapSceneModePane
    : public ln::EditorPane
{
public:
    void init();
    void setTileset(ln::Tileset* tileset);

protected:
    virtual void onRender(ln::UIRenderingContext* context) override;

private:
    Ref<ln::Tileset> m_tileset;
    Ref<ln::Material> m_material;
};

} // namespace lna

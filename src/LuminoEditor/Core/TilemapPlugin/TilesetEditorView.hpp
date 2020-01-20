#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"

namespace lna {

class TilesetView
    : public ln::UIControl
{
public:
    void setTileset(ln::Tileset* tileset);

protected:
    virtual void onRender(ln::UIRenderingContext* context) override;

private:
    Ref<ln::Tileset> m_tileset;
};

} // namespace lna

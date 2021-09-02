#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"

namespace lna {
class TilesetView;

class TilesetEditorModel
    : public lna::AssetEditorModel
{
public:
    ln::Result init();
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
    virtual Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(lna::EditorPaneKind kind) override;

private:
    Ref<TilesetView> m_tilesetView;
};

} // namespace lna

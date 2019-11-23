#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"

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

class TilesetEditor
    : public ln::AssetEditor
{
public:
    ln::Result init();
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
    virtual Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(ln::EditorPaneKind kind) override;

private:
    Ref<TilesetView> m_tilesetView;
};

class TilesetEditorPloxy
    : public ln::AssetEditorPloxy
{
public:
    virtual ln::String targetTypeName() override { return u"Tileset"; }
    virtual Ref<ln::AssetEditor> createEditor() override;
};

} // namespace lna

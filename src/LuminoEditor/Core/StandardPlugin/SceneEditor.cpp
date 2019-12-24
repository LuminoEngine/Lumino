#include "SceneEditor.hpp"

void SceneEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
}

void SceneEditor::onClosed()
{
}

//==============================================================================
// SceneEditorPlugin

SceneEditorPlugin::SceneEditorPlugin()
{

}

ln::Ref<lna::AssetEditor> SceneEditorPlugin::createEditor()
{
    return ln::makeObject<SceneEditor>();
}

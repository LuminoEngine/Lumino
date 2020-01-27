
#include "../Project/PluginManager.hpp"
#include "../App/EditorContext.hpp"
#include "../App/MainWindow.hpp"
#include "TilemapSceneNavigator.hpp"
#include "TilemapSceneEditor.hpp"
#include "TilemapSceneEditorModule.hpp"

namespace lna {
  
//==============================================================================
// TilemapSceneEditorPloxy

Ref<lna::AssetEditorModel> TilemapSceneEditorPloxy::createAssetEditorModel()
{
    return ln::makeObject<TilemapSceneEditor>();
}

//==============================================================================
// TilemapSceneEditorExtensionModule

void TilemapSceneEditorExtensionModule::onActivate(lna::EditorContext* context)
{
    m_navigator = ln::makeObject<TilemapSceneNavigator>();
    context->mainWindow()->navigatorManager()->addNavigator(m_navigator);

    m_editorPloxy = ln::makeObject<TilemapSceneEditorPloxy>();
    context->pluginManager()->registerAssetEditorFactory(m_editorPloxy);
}

void TilemapSceneEditorExtensionModule::onDeactivate(lna::EditorContext* context)
{
    context->pluginManager()->unregisterAssetEditorFactory(m_editorPloxy);
}

} // namespace lna


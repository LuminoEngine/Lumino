
#include <Workspace.hpp>
#include <Project.hpp>
#include <AssetDatabase.hpp>
#include <PluginManager.hpp>
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilesetNavigator.hpp"
#include "TilesetEditor.hpp"
#include "TilesetExtensionModule.hpp"

namespace lna {

//==============================================================================
// TilesetExtensionModule

void TilesetExtensionModule::onActivate(lna::EditorContext* context)
{
    m_navigator = ln::makeObject<TilesetNavigator>();
    context->mainWindow()->navigatorManager()->addNavigator(m_navigator);

    m_editorPloxy = ln::makeObject<TilesetEditorPloxy>();
    context->pluginManager()->addAssetEditorPloxy(m_editorPloxy);
}

void TilesetExtensionModule::onDeactivate(lna::EditorContext* context)
{
}

} // namespace lna

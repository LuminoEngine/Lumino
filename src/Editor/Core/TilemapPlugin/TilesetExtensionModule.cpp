
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../Project/PluginManager.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilesetNavigator.hpp"
#include "TilesetEditorModel.hpp"
#include "TilesetExtensionModule.hpp"

namespace lna {

//==============================================================================
// TilesetExtensionModule

void TilesetExtensionModule::onActivate(lna::EditorContext* context)
{
    m_navigator = ln::makeObject<TilesetNavigator>();
    context->mainWindow()->navigatorManager()->addNavigator(m_navigator);

	m_editorModelFactory = ln::makeObject<TilesetEditorModelFactory>();
    context->pluginManager()->registerAssetEditorFactory(m_editorModelFactory);
}

void TilesetExtensionModule::onDeactivate(lna::EditorContext* context)
{
	context->pluginManager()->unregisterAssetEditorFactory(m_editorModelFactory);
}

//==============================================================================
// TilesetEditorModelFactory

Ref<lna::AssetEditorModel> TilesetEditorModelFactory::createAssetEditorModel()
{
	return ln::makeObject<TilesetEditorModel>();
}


} // namespace lna

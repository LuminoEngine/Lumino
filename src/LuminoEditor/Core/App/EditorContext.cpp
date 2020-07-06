
#include "../Project/Project.hpp"
#include "../Project/PluginManager.hpp"
#include "../Project/AssetDatabase.hpp"
#include "EditorContext.hpp"
#include "MainWindow.hpp"
#include "NavigatorManager.hpp"
#include "DocumentManager.hpp"
#include "Application.hpp"

namespace lna {

EditorContext* EditorContext::current()
{
	return EditorApplication::instance()->editorContext();
}

NavigatorManager* EditorContext::navigatorManager() const
{
	return mainWindow()->navigatorManager();
}

DocumentManager* EditorContext::documentManager() const
{
	return mainWindow()->documentManager();
}

Project* EditorContext::mainProject() const
{
    return application()->mainProject();
}

PluginManager* EditorContext::pluginManager() const
{
    return EditorApplication::instance()->workspace()->mainPluginManager();
}

AssetDatabase* EditorContext::assetDatabase() const
{
	return EditorApplication::instance()->workspace()->mainAssetDatabase();
}

//void EditorContext::deactivateAllExtensions()
//{
//}

} // namespace lna


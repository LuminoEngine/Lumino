
#include "../Project.hpp"
#include "../PluginManager.hpp"
#include "EditorContext.hpp"
#include "Application.hpp"

namespace lna {

EditorContext* EditorContext::current()
{
	return EditorApplication::instance()->editorContext();
}

Project* EditorContext::mainProject() const
{
    return application()->mainProject();
}

AssetDatabase* EditorContext::assetDatabase() const
{
	return mainProject()->assetDatabase();
}

//void EditorContext::deactivateAllExtensions()
//{
//}

} // namespace lna


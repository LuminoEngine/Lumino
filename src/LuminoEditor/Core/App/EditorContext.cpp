
#include "../Project.hpp"
#include "../PluginManager.hpp"
#include "EditorContext.hpp"
#include "Application.hpp"

namespace lna {


Project* EditorContext::mainProject() const
{
    return application()->mainProject();
}

//void EditorContext::deactivateAllExtensions()
//{
//}

} // namespace lna


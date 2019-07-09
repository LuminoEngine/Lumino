#include "Project.hpp"
#include "PluginManager.hpp"

namespace lna {

//==============================================================================
// PluginManager

PluginManager::PluginManager()
    : m_ownerProject(nullptr)
    , m_pluginLibs()
{
}

ln::Result PluginManager::init(Project* owner)
{
    m_ownerProject = owner;
    return true;
}

void PluginManager::reloadPlugins()
{
    for (auto& file : m_pluginLibs) {
        file->unload();
    }
    m_pluginLibs.clear();

    auto files = ln::FileSystem::getFiles(m_ownerProject->localPluginDir(), u"*.dll");
    for (auto& file : files) {
        m_pluginLibs.add(ln::detail::DllLoader::load(file));
    }
}

} // namespace lna


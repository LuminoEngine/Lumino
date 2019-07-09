#pragma once
#include <LuminoCore/IO/DllLoader.hpp>

namespace lna {
class Project;

class PluginManager
	: public ln::RefObject
{
public:
    PluginManager();
    ln::Result init(Project* owner);

    void reloadPlugins();

private:
    Project* m_ownerProject;
    ln::List<Ref<ln::detail::DllLoader>> m_pluginLibs;
};


} // namespace lna

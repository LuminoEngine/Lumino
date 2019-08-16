#pragma once
#include <LuminoCore/IO/DllLoader.hpp>
#include <LuminoEditor/Plugin.hpp>

namespace lna {
class Project;
class StandardPluginModule;

class PluginManager
	: public ln::RefObject
{
public:
    PluginManager();
    ln::Result init(Project* owner);

    void reloadPlugins();

    ln::List<ln::IAssetNavigatorExtension*> getAssetNavigatorExtensions() const;
    ln::List<std::pair<ln::IAssetImporterEditorExtension*, Ref<ln::AssetImporter>>> getAssetImporterExtensions(const ln::Path& assetSourceFilePath) const;
    ln::List<std::pair<ln::IAssetEditorExtension*, Ref<ln::AssetEditor>>> geAssetEditorExtensions(const ln::String& assetType) const;

private:
    Project* m_ownerProject;
    ln::List<Ref<ln::detail::DllLoader>> m_pluginLibs;
    ln::List<ln::IPluginModule*> m_pluginModules;
    ln::Ref<StandardPluginModule> m_standartPluginModule;
};



} // namespace lna

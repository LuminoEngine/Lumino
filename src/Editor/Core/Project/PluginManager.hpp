#pragma once
#include <LuminoCore/IO/DllLoader.hpp>
#include <LuminoEditor/Plugin.hpp>

namespace lna {
class Project;
class StandardPluginModule;
class TilesetExtensionModule;
class TilemapSceneEditorExtensionModule;
class EditorContext;
class AssetEditorModel;

class PluginManager
	: public ln::RefObject
{
public:
    PluginManager();
    ln::Result init(Project* owner);

    void reloadPlugins();
    void activateAllExtensions(EditorContext* context);
    void deactivateAllExtensions(EditorContext* context);

    void registerAssetEditorFactory(ln::AssetEditorModelFactory* value);
    void unregisterAssetEditorFactory(ln::AssetEditorModelFactory* value);

    //ln::List<ln::IAssetNavigatorExtension*> getAssetNavigatorExtensions() const;
    //ln::List<std::pair<ln::IAssetImporterEditorExtension*, Ref<ln::AssetImporter>>> getAssetImporterExtensions(const ln::Path& assetSourceFilePath) const;
    //ln::List<std::pair<ln::IAssetEditorExtension*, Ref<lna::AssetEditorModel>>> geAssetEditorExtensions(const ln::String& assetType) const;
    ln::List<ln::AssetEditorModelFactory*> findAssetEditorModelFactory(const ln::TypeInfo* assetType) const;

private:
    Project* m_ownerProject;
    ln::List<Ref<ln::detail::DllLoader>> m_pluginLibs;
    ln::List<ln::IPluginModule*> m_pluginModules;
    ln::List<ln::AssetEditorModelFactory*> m_assetEditorFactories;

    ln::Ref<StandardPluginModule> m_standartPluginModule;
    ln::Ref<TilesetExtensionModule> m_tilesetExtensionModule;
    ln::Ref<TilemapSceneEditorExtensionModule> m_tilemapSceneEditorExtensionModule;
};



} // namespace lna

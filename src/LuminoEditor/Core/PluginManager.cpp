#include "Project.hpp"
#include "PluginManager.hpp"
#include "StandardPlugin/AssetBrowserNavigator.hpp"
#include "StandardPlugin/StandardPluginModule.hpp"
#include "TilemapPlugin/TilesetEditor.hpp"
#include "TilemapPlugin/TilemapSceneEditor.hpp"

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
    m_standartPluginModule = ln::makeObject<StandardPluginModule>();
    m_tilesetEditorExtensionModule = ln::makeObject<TilesetEditorExtensionModule>();
    m_tilemapSceneEditorExtensionModule = ln::makeObject<TilemapSceneEditorExtensionModule>();
    return true;
}

void PluginManager::reloadPlugins()
{
    m_pluginModules.clear();

    for (auto& file : m_pluginLibs) {
        file->unload();
    }
    m_pluginLibs.clear();

#if 0
    // Load modules
    if (ln::FileSystem::existsDirectory(m_ownerProject->localPluginDir())) {
        auto files = ln::FileSystem::getFiles(m_ownerProject->localPluginDir(), u"*.dll");
        for (auto& file : files) {
            auto lib = ln::detail::DllLoader::load(file);
            auto func = reinterpret_cast<ln::GetModuleClassFunc*>(lib->getProcAddress("ModuleClass"));
            if (func) {
                m_pluginModules.add(func());
                m_pluginLibs.add(lib);
            }
        }
    }
#endif

    m_pluginModules.add(m_standartPluginModule);
    m_pluginModules.add(m_tilesetEditorExtensionModule);
    m_pluginModules.add(m_tilemapSceneEditorExtensionModule);
    
}

void PluginManager::activateAllExtensions(EditorContext* context)
{
    for (auto& module : m_pluginModules) {
        module->onActivate(context);
    }
}

void PluginManager::deactivateAllExtensions(EditorContext* context)
{
    for (auto& module : m_pluginModules) {
        module->onDeactivate(context);
    }
}

void PluginManager::addAssetEditorPloxy(ln::AssetEditorPloxy* ploxy)
{
    m_assetEditorPloxy.add(ploxy);
}

void PluginManager::removeAssetEditorPloxy(ln::AssetEditorPloxy* ploxy)
{
    m_assetEditorPloxy.remove(ploxy);
}

ln::List<ln::IAssetNavigatorExtension*> PluginManager::getAssetNavigatorExtensions() const
{
    ln::List<ln::IAssetNavigatorExtension*> result;
    for (auto& module : m_pluginModules) {
        int count = module->getEditorExtensionCount();
        for (int i = 0; i < count; i++) {
            auto ext = module->getEditorExtension(i);
            if (ext && ext->getExtensionType() == ln::EditorExtensionType::AssetNavigator) {
                result.add(static_cast<ln::IAssetNavigatorExtension*>(ext));
            }
        }
    }

    return result;
}

ln::List<std::pair<ln::IAssetImporterEditorExtension*, Ref<ln::AssetImporter>>> PluginManager::getAssetImporterExtensions(const ln::Path& assetSourceFilePath) const
{
    ln::List<std::pair<ln::IAssetImporterEditorExtension*, Ref<ln::AssetImporter>>> result;
    for (auto& module : m_pluginModules) {
        int count = module->getEditorExtensionCount();
        for (int i = 0; i < count; i++) {
            auto ext = module->getEditorExtension(i);
            if (ext && ext->getExtensionType() == ln::EditorExtensionType::AssetImporter) {
                auto importerExt = static_cast<ln::IAssetImporterEditorExtension*>(ext);
                if (auto importer = importerExt->createImporter(assetSourceFilePath.c_str())) {
                    result.add({ importerExt, importer });
                }
            }
        }
    }
    return result;
} 

ln::List<std::pair<ln::IAssetEditorExtension*, Ref<ln::AssetEditor>>> PluginManager::geAssetEditorExtensions(const ln::String& assetType) const
{
    ln::List<std::pair<ln::IAssetEditorExtension*, Ref<ln::AssetEditor>>> result;
    for (auto& module : m_pluginModules) {
        int count = module->getEditorExtensionCount();
        for (int i = 0; i < count; i++) {
            auto ext = module->getEditorExtension(i);
            if (ext && ext->getExtensionType() == ln::EditorExtensionType::AssetEditor) {
                auto editorExt = static_cast<ln::IAssetEditorExtension*>(ext);
                if (assetType == editorExt->typeKeyword()) {
                    if (auto editor = editorExt->createEditor()) {
                        result.add({ editorExt, editor });
                    }
                }
            }
        }
    }
    return result;
}

ln::List<ln::AssetEditorPloxy*> PluginManager::geAssetEditorPloxy(const ln::String& assetType) const
{
    ln::List<ln::AssetEditorPloxy*> result;
    for (auto& ploxy : m_assetEditorPloxy) {
        if (assetType == ploxy->targetTypeName()) {
            result.add(ploxy);
        }
    }
    return result;
}

} // namespace lna


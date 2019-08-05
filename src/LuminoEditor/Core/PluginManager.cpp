#include "Project.hpp"
#include "PluginManager.hpp"
#include "StandardPlugin/TilesetEditor.hpp"

namespace lna {


//==============================================================================
// StandardTextureImporterExtension

int StandardTextureImporterExtension::matchFilePath(const ln::Path& filePath)
{
    // TODO: static メソッドにしたい
    auto importer = ln::makeObject<ln::TextureImporter>();
    if (importer->testSupportedExtensions(filePath)) {
        return BasePriority;
    }
    return 0;
}

Ref<ln::AssetImporter> StandardTextureImporterExtension::createImporter(const ln::Char* assetSourceFilePath)
{
    auto importer = ln::makeObject<ln::TextureImporter>();
    if (!importer->testSupportedExtensions(assetSourceFilePath)) {
        return nullptr;
    }
    return importer;
}


//==============================================================================
// StandardPluginModule

StandardPluginModule::StandardPluginModule()
{
    {
        auto obj = ln::makeObject<StandardTextureImporterExtension>();
        m_editorExtensionInstances.add(obj);
        m_editorExtensions.add(obj);
    }
    {
        auto obj = ln::makeObject<TilesetNavigatorExtension>();
        m_editorExtensionInstances.add(obj);
        m_editorExtensions.add(obj);
    }
}

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
    return true;
}

void PluginManager::reloadPlugins()
{
    m_pluginModules.clear();

    for (auto& file : m_pluginLibs) {
        file->unload();
    }
    m_pluginLibs.clear();

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

    m_pluginModules.add(m_standartPluginModule);
    
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

ln::List<std::pair<ln::IDocumentEditorExtension*, Ref<ln::AssetEditorViewModel>>> PluginManager::geAssetEditorExtensions(const ln::String& assetType) const
{
    ln::List<std::pair<ln::IDocumentEditorExtension*, Ref<ln::AssetEditorViewModel>>> result;
    for (auto& module : m_pluginModules) {
        int count = module->getEditorExtensionCount();
        for (int i = 0; i < count; i++) {
            auto ext = module->getEditorExtension(i);
            if (ext && ext->getExtensionType() == ln::EditorExtensionType::DocumentEditor) {
                auto editorExt = static_cast<ln::IDocumentEditorExtension*>(ext);
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

} // namespace lna


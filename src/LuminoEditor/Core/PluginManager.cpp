#include "Project.hpp"
#include "PluginManager.hpp"

namespace lna {


//==============================================================================
// StandardTextureImporterExtension

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
    auto files = ln::FileSystem::getFiles(m_ownerProject->localPluginDir(), u"*.dll");
    for (auto& file : files) {
        m_pluginLibs.add(ln::detail::DllLoader::load(file));
    }


    m_pluginModules.add(m_standartPluginModule);
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

} // namespace lna


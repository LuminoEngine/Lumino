#pragma once
#include <LuminoCore/IO/DllLoader.hpp>
#include <LuminoEditor/Plugin.hpp>

namespace lna {
class Project;

class StandardTextureImporterExtension
    : public ln::Object
    , public ln::IAssetImporterEditorExtension
{
public:
    virtual const ln::Char* id() const { return u"D8F77953-DEB1-4160-9C50-54B5FAF13777"; }
    virtual const ln::Char* displayName() const { return u"Standard texture importer"; }
    virtual int matchFilePath(const ln::Path& filePath) override;
    virtual Ref<ln::AssetImporter> createImporter(const ln::Char* assetSourceFilePath) override;
};

class StandardPluginModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    StandardPluginModule();
    virtual int getEditorExtensionCount() { return m_editorExtensions.size(); }
    virtual ln::IEditorExtension* getEditorExtension(int index) { return m_editorExtensions[index]; }

private:
    ln::List<Ref<ln::Object>> m_editorExtensionInstances;
    ln::List<ln::IEditorExtension*> m_editorExtensions;
};

class PluginManager
	: public ln::RefObject
{
public:
    PluginManager();
    ln::Result init(Project* owner);

    void reloadPlugins();

    ln::List<ln::IAssetNavigatorExtension*> getAssetNavigatorExtensions() const;
    ln::List<std::pair<ln::IAssetImporterEditorExtension*, Ref<ln::AssetImporter>>> getAssetImporterExtensions(const ln::Path& assetSourceFilePath) const;
    ln::List<std::pair<ln::IAssetEditorExtension*, Ref<ln::AssetEditorViewModel>>> geAssetEditorExtensions(const ln::String& assetType) const;

private:
    Project* m_ownerProject;
    ln::List<Ref<ln::detail::DllLoader>> m_pluginLibs;
    ln::List<ln::IPluginModule*> m_pluginModules;
    ln::Ref<StandardPluginModule> m_standartPluginModule;
};



} // namespace lna

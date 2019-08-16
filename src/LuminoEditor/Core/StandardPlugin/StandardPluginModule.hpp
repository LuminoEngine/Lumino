#pragma once
#include <LuminoEditor/Plugin.hpp>

namespace lna {

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
    virtual void onActivate(lna::EditorContext* context) {}
    virtual void onDeactivate(lna::EditorContext* context) {}

private:
    ln::List<Ref<ln::Object>> m_editorExtensionInstances;
    ln::List<ln::IEditorExtension*> m_editorExtensions;
};

} // namespace lna

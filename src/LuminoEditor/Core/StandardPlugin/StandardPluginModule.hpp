#pragma once
#include <LuminoEditor/Plugin.hpp>

namespace lna {

//class StandardTextureImporterExtension
//    : public ln::Object
//    , public ln::IAssetImporterEditorExtension
//{
//public:
//    virtual const ln::Char* id() const { return u"D8F77953-DEB1-4160-9C50-54B5FAF13777"; }
//    virtual const ln::Char* displayName() const { return u"Standard texture importer"; }
//    virtual int matchFilePath(const ln::Path& filePath) override;
//    virtual Ref<ln::AssetImporter> createImporter(const ln::Char* assetSourceFilePath) override;
//};

class StandardPluginModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    StandardPluginModule();
    const ln::Char* moduleId() const override { return u"1A07666A-50E3-4972-BF7E-7418CCA118F2"; }
    const ln::Char* moduleDisplayName() const override { return u"StandardPluginModule"; }
    //virtual int getEditorExtensionCount() { return m_editorExtensions.size(); }
    //virtual ln::IEditorExtension* getEditorExtension(int index) { return m_editorExtensions[index]; }
    void onActivate(lna::EditorContext* context) override;
    void onDeactivate(lna::EditorContext* context) override;

private:
    //ln::List<Ref<ln::Object>> m_editorExtensionInstances;
    //ln::List<ln::IEditorExtension*> m_editorExtensions;
};

} // namespace lna

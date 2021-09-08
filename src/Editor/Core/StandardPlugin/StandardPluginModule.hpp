#pragma once
#include <LuminoEditor/Plugin.hpp>

namespace lna {
class AssetBrowserNavigator;


class LevelEditorModelFactory
    : public ln::AssetEditorModelFactory
{
public:
    bool checkTargetType(const ln::TypeInfo* assetType) override;
    Ref<lna::AssetEditorModel> createAssetEditorModel() override;
};

class StandardPluginModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    StandardPluginModule();
    const ln::Char* moduleId() const override { return _TT("1A07666A-50E3-4972-BF7E-7418CCA118F2"); }
    const ln::Char* moduleDisplayName() const override { return _TT("StandardPluginModule"); }
    void onActivate(lna::EditorContext* context) override;
    void onDeactivate(lna::EditorContext* context) override;

private:
    Ref<AssetBrowserNavigator> m_navigator;
    Ref<LevelEditorModelFactory> m_levelEditorModelFactory;
    //ln::List<Ref<ln::Object>> m_editorExtensionInstances;
    //ln::List<ln::IEditorExtension*> m_editorExtensions;
};

} // namespace lna

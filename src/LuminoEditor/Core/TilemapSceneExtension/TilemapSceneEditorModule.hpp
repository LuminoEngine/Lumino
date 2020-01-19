#pragma once
#include <LuminoEditor/Plugin.hpp>

namespace lna {
class TilemapSceneNavigator;

class TilemapSceneEditorPloxy
    : public ln::AssetEditorModelFactory
{
public:
    virtual ln::String targetTypeName() override { return u"Scene"; }
    virtual Ref<lna::AssetEditorModel> createAssetEditorModel() override;
};

class TilemapSceneEditorExtensionModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    virtual const ln::Char* moduleId() const override { return u"2E276188-3034-4C29-BE7E-15FB30C5515A"; }
    virtual const ln::Char* moduleDisplayName() const override { return u"TilemapSceneEditorExtensionModule"; }
    virtual void onActivate(lna::EditorContext* context) override;
    virtual void onDeactivate(lna::EditorContext* context) override;

private:
    Ref<TilemapSceneNavigator> m_navigator;
    Ref<TilemapSceneEditorPloxy> m_editorPloxy;
};

} // namespace lna 


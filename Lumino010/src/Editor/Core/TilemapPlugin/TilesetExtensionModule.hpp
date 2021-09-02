#pragma once
#include <LuminoEditor/Plugin.hpp>

namespace lna {
class TilesetNavigator;
class TilesetEditorModelFactory;

class TilesetExtensionModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    virtual const ln::Char* moduleId() const override { return u"7BDAD7F8-E8A1-4B3A-9883-3700A6315A2A"; }
    virtual const ln::Char* moduleDisplayName() const override { return u"TilesetExtensionModule"; }
    virtual void onActivate(lna::EditorContext* context) override;
    virtual void onDeactivate(lna::EditorContext* context) override;

private:
    Ref<TilesetNavigator> m_navigator;
    Ref<TilesetEditorModelFactory> m_editorModelFactory;
};

class TilesetEditorModelFactory
	: public ln::AssetEditorModelFactory
{
public:
	virtual ln::String targetTypeName() override { return u"Tileset"; }
	virtual Ref<lna::AssetEditorModel> createAssetEditorModel() override;
};

} // namespace lna


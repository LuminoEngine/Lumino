#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"


namespace lna {
class TilesetNavigator;

class TilesetEditorExtensionModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    virtual void onActivate(lna::EditorContext* context) override;
    virtual void onDeactivate(lna::EditorContext* context) override;

private:
    Ref<TilesetNavigator> m_navigator;
};

} // namespace lna


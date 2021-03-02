#pragma once
#include <LuminoEngine/UI/ImGuiIntegration.hpp>
#include <LuminoEngine/Scene/Common.hpp>

namespace ln {
namespace detail {

class EditorViewportToolPane
    : public ImGuiDockPane
{
public:
    EditorViewportToolPane();
    bool init(UIMainWindow* mainWindow);

protected:
    void onGui() override;

private:
    UIMainWindow* m_mainWindow;
    Ref<WorldRenderView> m_renderView;
    Ref<RenderTargetTexture> m_renderTarget;
};

} // namespace detail
} // namespace ln


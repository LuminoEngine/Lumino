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
    bool onUIEvent(UIEventArgs* e) override;

private:
    UIMainWindow* m_mainWindow;
    Ref<WorldRenderView> m_renderView;
    Ref<RenderTargetTexture> m_renderTarget;
    Ref<Camera> m_camera;
    Ref<CameraOrbitControlComponent> m_cameraOrbitControlComponent;
    bool m_mouseHoverd;
};

} // namespace detail
} // namespace ln



#include "Internal.hpp"
#include <LuminoGraphics/SamplerState.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/CameraOrbitControlComponent.hpp>
#include <LuminoEngine/PostEffect/FilmicPostEffect.hpp>
#include "../Engine/EngineManager.hpp"
#include "EditorViewportToolPane.hpp"

namespace ln {
namespace detail {

//==============================================================================
// EditorViewportToolPane

EditorViewportToolPane::EditorViewportToolPane()
    : m_mouseHoverd(true)
{}

bool EditorViewportToolPane::init(UIMainWindow* mainWindow)
{
	if (!Object::init()) return false;

    m_camera = makeObject<Camera>();

    m_mainWindow = mainWindow;
	m_renderView = makeObject<WorldRenderView>();
	m_renderView->setTargetWorld(detail::EngineDomain::engineManager()->mainWorld());
	m_renderView->setCamera(m_camera);
	m_renderView->setClearMode(SceneClearMode::ColorAndDepth);

    m_renderView->setGuideGridEnabled(true);
    m_renderView->setGizmoEnabled(true);
    m_renderView->finishingProcess()->setAntialiasEnabled(false);

    m_cameraOrbitControlComponent = makeObject<CameraOrbitControlComponent>();
    m_camera->addComponent(m_cameraOrbitControlComponent);
	return true;
}

void EditorViewportToolPane::onGui()
{
    GraphicsCommandList* graphicsContext = m_mainWindow->m_renderingGraphicsContext;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const ImVec2 contentSize = ImGui::GetContentRegionAvail();

    //if (renderView)
    {
    //    m_frameWindow->m_contentArea.set(
    //        window->ContentRegionRect.Min.x, window->ContentRegionRect.Min.y,
    //        m_contentSize.width, m_contentSize.height);

        //m_frameWindow->updateLayoutTreeInternal(m_contentSize);
        m_renderView->setActualSize(Size(contentSize.x, contentSize.y));

        m_renderTarget = RenderTargetTexture::realloc(m_renderTarget, contentSize.x, contentSize.y, TextureFormat::RGBA8, false, SamplerState::pointClamp());
        m_renderView->render(graphicsContext, m_renderTarget);
    }


    ImGui::Image(m_renderTarget, contentSize);

    m_mouseHoverd = false;
    if (ImGui::IsMouseHoveringRect(window->ContentRegionRect.Min, window->ContentRegionRect.Max)) {
        m_mouseHoverd = true;
    //    Vector2 pos(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

    //    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) ||
    //        ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
    //        m_cameraOrbitControlComponent->handleMouseUp();
    //        
    //    }
    //    else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    //        m_cameraOrbitControlComponent->startRotate(pos);

    //        //ImGuiMouseButton_Left = 0,
    //        //     = 1,
    //        //    ImGuiMouseButton_Middle = 2,
    //    }

    //    m_cameraOrbitControlComponent->handleMouseMove(pos);
    }

    //window->

    //if (ImGui::mouse(ImGuiMouseButton_Right)) {
    //    std::cout << "Down" << std::endl;

    //    //ImGuiMouseButton_Left = 0,
    //    //     = 1,
    //    //    ImGuiMouseButton_Middle = 2,
    //}


}

bool EditorViewportToolPane::onUIEvent(UIEventArgs* e)
{
    if (m_mouseHoverd && m_cameraOrbitControlComponent) {
        m_cameraOrbitControlComponent->handleUIEvent(e);
    }
    return e->handled;
}

} // namespace detail
} // namespace ln


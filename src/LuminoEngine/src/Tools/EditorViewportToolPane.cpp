
#include "Internal.hpp"
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include "../Engine/EngineManager.hpp"
#include "EditorViewportToolPane.hpp"

namespace ln {
namespace detail {

//=============================================================================
// EditorViewportToolPane

EditorViewportToolPane::EditorViewportToolPane()
{}

bool EditorViewportToolPane::init(UIMainWindow* mainWindow)
{
	if (!Object::init()) return false;

    m_mainWindow = mainWindow;
	m_renderView = makeObject<WorldRenderView>();
	m_renderView->setTargetWorld(detail::EngineDomain::engineManager()->mainWorld());
	m_renderView->setCamera(detail::EngineDomain::engineManager()->mainCamera());
	m_renderView->setClearMode(SceneClearMode::ColorAndDepth);

	return true;
}

void EditorViewportToolPane::onGui()
{
    GraphicsContext* graphicsContext = m_mainWindow->m_renderingGraphicsContext;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const ImVec2 contentSize = ImGui::GetContentRegionAvail();

    //if (renderView)
    {
    //    m_frameWindow->m_contentArea.set(
    //        window->ContentRegionRect.Min.x, window->ContentRegionRect.Min.y,
    //        m_contentSize.width, m_contentSize.height);

        //m_frameWindow->updateLayoutTreeInternal(m_contentSize);

        m_renderTarget = RenderTargetTexture::realloc(m_renderTarget, contentSize.x, contentSize.y, TextureFormat::RGBA8, false, SamplerState::pointClamp());
        m_renderView->render(graphicsContext, m_renderTarget);
    }

    ImGui::Image(m_renderTarget, contentSize);
}

} // namespace detail
} // namespace ln


#include "Internal.hpp"
#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "MainViewportToolPane.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MainViewportToolPane
	
void MainViewportToolPane::onGui()
{

    UIRenderView* renderView = m_frameWindow->m_renderView;
    GraphicsCommandList* graphicsContext = m_frameWindow->m_renderingGraphicsContext;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    const ImVec2 contentSize = ImGui::GetContentRegionAvail();
    m_contentSize.set(contentSize.x, contentSize.y);

    if (renderView)
    {
        m_frameWindow->m_contentArea.set(
            window->ContentRegionRect.Min.x, window->ContentRegionRect.Min.y,
            m_contentSize.width, m_contentSize.height);

        m_frameWindow->updateLayoutTreeInternal(m_contentSize);
        m_mainViewportRenderTarget = RenderTargetTexture::realloc(m_mainViewportRenderTarget, contentSize.x, contentSize.y, TextureFormat::RGBA8, false, SamplerState::pointClamp());
        renderView->render(graphicsContext, m_mainViewportRenderTarget);
    }

    ImGui::Image(m_mainViewportRenderTarget, contentSize);

    if (m_resetCount > 0) {
        m_resetCount--;
    }
}

} // namespace detail
} // namespace ln


#pragma once
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>

namespace ln {
namespace detail {

class MainViewportToolPane
    : public ImGuiDockPane
{
public:
    MainViewportToolPane()
        : m_frameWindow(nullptr)
        , m_mainViewportRenderTarget(nullptr)
        , m_resetCount(1)
    {}

    bool isSizeReseting() const { return m_resetCount > 0; }

    void prepare(UIFrameWindow* frameWindow)
    {
        m_frameWindow = frameWindow;
    }

protected:
    void onGui() override;

private:
    UIFrameWindow* m_frameWindow;
    Ref<RenderTargetTexture> m_mainViewportRenderTarget;
    int m_resetCount;
    Size m_contentSize;
};

} // namespace detail
} // namespace ln


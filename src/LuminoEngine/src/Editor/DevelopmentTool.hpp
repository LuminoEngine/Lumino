#pragma once
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>

namespace ln {
class UIEventArgs;
class UIElement;
class UIMainWindow;
class UISplitter;

namespace detail {
class EngineManager;
class MainViewportToolPane;
class EditorViewportToolPane;
class ProfilerToolPane;

class RuntimeEditor
    : public RefObject
{
public:
    enum class Mode
    {
        Hidden,
        Activated,
    };

    RuntimeEditor();
    void init(EngineManager* manager, UIMainWindow* window);
    void dispose();
    void toggleMode();
    void setMode(Mode value);
    void updateFrame();

    const Ref<MainViewportToolPane>& mainViewportToolPane() const { return m_mainViewportToolPane; }
    const Ref<EditorViewportToolPane>& editorViewportToolPane() const { return m_editorViewportToolPane; }

private:
    void attach();
    void detach();
    void handleImGuiDebugLayer(UIEventArgs* e);

    EngineManager* m_manager;
    Mode m_mode;

    UIMainWindow* m_window;
    //Ref<UISplitter> m_splitter;
    //Ref<UIControl> m_mainContentsPane;
    //Ref<UIElement> m_toolPane;
    //Ref<UIFrameWindow> m_toolWindow;

    Size m_gameModeWindowSize;
    Size m_toolModeWindowSize;


    Ref<MainViewportToolPane> m_mainViewportToolPane;
    Ref<ImGuiDockPane> m_pane2;
    Ref<ImGuiDockPane> m_pane3;
    Ref<ProfilerToolPane> m_profilerToolPane;
    Ref<ImGuiDockPane> m_pane5;
    Ref<ImGuiDockPane> m_pane6;
    Ref<EditorViewportToolPane> m_editorViewportToolPane;
};

} // namespace detail
} // namespace ln


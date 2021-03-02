﻿#pragma once
#include <LuminoEngine/UI/UIFrameWindow.hpp>

namespace ln {
class UIEventArgs;
class UIElement;
class UIMainWindow;
class UISplitter;

namespace detail {


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
};

} // namespace detail
} // namespace ln


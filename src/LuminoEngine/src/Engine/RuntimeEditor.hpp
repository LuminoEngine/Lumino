#pragma once

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
    RuntimeEditor();
    void init(EngineManager* manager, UIMainWindow* window);


private:
    void handleImGuiDebugLayer(UIEventArgs* e);

    EngineManager* m_manager;
    UIMainWindow* m_window;
    Ref<UISplitter> m_splitter;
    Ref<UIElement> m_toolPane;
};

} // namespace detail
} // namespace ln


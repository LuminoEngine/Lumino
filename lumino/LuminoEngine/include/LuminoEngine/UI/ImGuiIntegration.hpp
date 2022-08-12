#pragma once
#include <imgui.h>
#include <imgui_internal.h>

struct ImGuiContext;

namespace ln {
class GraphicsCommandList;
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class Texture2D;
class RenderTargetTexture;
class RenderPass;
class Shader;
class ImGuiDockPane;
class UIEventArgs;
class UIFrameWindow;
//class UIImGuiContext;
struct PlatformEventArgs;

class ImGuiDockManager;
class ImGuiMenuBar;
class ImGuiToolBar;
class ImGuiDockPane;

enum ImGuiDockPlacement {
    Floating,
    MainView,
    Left,
    Right,
    //Top,
    Bottom,
    InnerLeft,
    DebugView,
};

namespace detail {

class ImGuiIntegration {
public:
    bool init(UIFrameWindow* frameWindow);
    void dispose();
    void updateFrame(float elapsedSeconds);
    void prepareRender(float width, float height);
    void renderContents();
    void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* target);
    bool handlePlatformEvent(const PlatformEventArgs& e);

    bool handleUIEvent(UIEventArgs* e);

    UIFrameWindow* frameWindow() const { return m_frameWindow; }
    const Ref<ImGuiDockManager>& dockManager() const { return m_dockManager; }
    //void setImGuiContext(UIImGuiContext* context);

private:
    UIFrameWindow* m_frameWindow;
    ::ImGuiContext* m_imgui;
    Ref<Texture2D> m_fontTexture;
    int m_vertexBufferSize = 5000;
    int m_indexBufferSize = 10000;
    Ref<VertexLayout> m_vertexLayout;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
    Ref<Shader> m_shader;
    Ref<RenderPass> m_renderPass;

    Ref<ImGuiDockManager> m_dockManager;
    
    //Ref<UIImGuiContext> m_userContext;

};

} // namespace detail

//class UIImGuiContext : public Object {
//protected:
//
//private:
//    friend class detail::ImGuiIntegration;
//};

/** Dock 周りの定型的な処理と、Beta リリースである API のラップを目的とする。 */
class ImGuiDockManager : public Object {
public:
    virtual ~ImGuiDockManager();
    const Array<Ref<ImGuiDockPane>>& dockPanes() const { return m_dockPanes; }
    void addMenuBar(ImGuiMenuBar* menuBar);
    void addToolBar(ImGuiToolBar* toolBar);
    void addDockPane(ImGuiDockPane* pane);

private:
    ImGuiDockManager();
    bool handleUIEvent(UIEventArgs* e);
    void updateDocks();

    detail::ImGuiIntegration* m_imguiIntegration;
    Array<Ref<ImGuiMenuBar>> m_menuBars;
    Array<Ref<ImGuiToolBar>> m_toolBars;
    Array<Ref<ImGuiDockPane>> m_dockPanes;
    bool m_dockLayoutResetRequired = true;
    ImGuiWindowFlags m_dockSpaceWindowFlags;

    friend class detail::ImGuiIntegration;
    friend class ImGuiDockPane;
};

class ImGuiMenuBar : public Object {
public:
    /**
     * GUI の構築時に呼び出されます。
     * 
     * この関数の実装内で ImGui::BeginMenuBar() と ImGui::EndMenuBar() を呼び出す必要があります。
     */
    virtual void onImGui();

private:
    ImGuiDockManager* m_dockManager;
    std::string m_key;
    friend class ImGuiDockManager;
};

class ImGuiToolBar : public Object {
public:

protected:
    virtual void onImGui();

private:
    ImGuiDockManager* m_dockManager;
    std::string m_key;
    friend class ImGuiDockManager;
};

/** Dock 周りの定型的な処理と、Beta リリースである API のラップを目的とする。 */
class ImGuiDockPane : public Object {
public:
    UIFrameWindow* frameWindow() const;

    void setInitialPlacement(ImGuiDockPlacement value);
    void close();

protected:
    virtual void onGui();
    virtual bool onUIEvent(UIEventArgs* e);

LN_CONSTRUCT_ACCESS:
    ImGuiDockPane();
    Result init();

private:
    void update();

    ImGuiDockManager* m_dockManager;
    std::string m_key;
    ImGuiDockPlacement m_initialPlacement;
    //Event<UIGeneralEventHandler> m_onGui;
    bool m_open;

    friend class ImGuiDockManager;
};
} // namespace ln

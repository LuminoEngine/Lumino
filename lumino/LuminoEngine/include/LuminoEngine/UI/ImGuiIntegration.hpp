#pragma once
#include <imgui.h>
#include <imgui_internal.h>

struct ImGuiContext;

namespace ln {
class GraphicsContext;
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

enum ImGuiDockPlacement
{
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
struct PlatformEventArgs;

class ImGuiIntegration
{
public:
	bool init(UIFrameWindow* frameWindow);
	void dispose();
    void updateFrame(float elapsedSeconds);
	void prepareRender(float width, float height);
	void render(GraphicsContext* graphicsContext, RenderTargetTexture* target);
    bool handlePlatformEvent(const detail::PlatformEventArgs& e);

	const List<Ref<ImGuiDockPane>>& dockPanes() const { return m_dockPanes; }
	void addDock(ImGuiDockPane* pane);
	void updateDocks(ImGuiID mainWindowId);
	bool handleUIEvent(UIEventArgs* e);

	UIFrameWindow* frameWindow() const { return m_frameWindow; }

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

	List<Ref<ImGuiDockPane>> m_dockPanes;
	bool m_dockLayoutResetRequired = true;
};

} // namespace detail


/** Dock 周りの定型的な処理と、Beta リリースである API のラップを目的とする。 */
class ImGuiDockPane
	: public Object
{
public:
	UIFrameWindow* frameWindow() const;

	void setInitialPlacement(ImGuiDockPlacement value);
	void close();

protected:
	virtual void onGui();
	virtual bool onUIEvent(UIEventArgs* e);

LN_CONSTRUCT_ACCESS:
	ImGuiDockPane();
	bool init();

private:
	void update();

	detail::ImGuiIntegration* m_imguiIntegration;
	std::string m_key;
	ImGuiDockPlacement m_initialPlacement;
	//Event<UIGeneralEventHandler> m_onGui;
	bool m_open;

	friend class detail::ImGuiIntegration;
};
} // namespace ln



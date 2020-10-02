#pragma once

struct ImGuiContext;

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class Texture2D;
class RenderPass;

namespace detail {
struct PlatformEventArgs;

class ImGuiIntegration
{
public:
	bool init();
	void dispose();
    void updateFrame(float elapsedSeconds);
	void prepareRender(float width, float height);
	void render(GraphicsContext* graphicsContext, RenderTargetTexture* target);
    bool handlePlatformEvent(const detail::PlatformEventArgs& e);

private:
	::ImGuiContext* m_imgui;
	Ref<Texture2D> m_fontTexture;
	int m_vertexBufferSize = 5000;
	int m_indexBufferSize = 10000;
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	Ref<Shader> m_shader;
	Ref<RenderPass> m_renderPass;
};

} // namespace detail
} // namespace ln



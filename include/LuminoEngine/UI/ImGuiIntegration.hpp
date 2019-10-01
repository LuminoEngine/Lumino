#pragma once

namespace ln {
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class Texture2D;

namespace detail {
struct PlatformEventArgs;

class ImGuiContext
{
public:
	bool init();
	void dispose();
    void updateFrame(float elapsedSeconds);
	void render(GraphicsContext* graphicsContext);
    bool handlePlatformEvent(const detail::PlatformEventArgs& e);

private:
	Ref<Texture2D> m_fontTexture;
	int m_vertexBufferSize = 5000;
	int m_indexBufferSize = 10000;
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	Ref<Shader> m_shader;
};

} // namespace detail
} // namespace ln



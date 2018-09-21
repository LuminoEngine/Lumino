#pragma once
#include "../Graphics/Common.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/DepthBuffer.hpp"
#include "../Shader/ShaderInterfaceFramework.hpp"

namespace ln {
class GraphicsContext;
namespace detail {
class RenderingManager;
class FrameBufferCache;
class DrawElementList;
class DrawElementListCollector;
class SceneRenderer;
}

struct FrameBuffer
{
	Ref<RenderTargetTexture> renderTarget[detail::MaxMultiRenderTargets];
	Ref<DepthBuffer> depthBuffer;
};

class RenderView
	: public Object
{
public:
	void initialize();

	// World の描画などでは RenderView のインスタンスを持ち回り、描画してほしい人が Manager を ここに add していく。
	void clearDrawElementListManagers();
	void addDrawElementListManager(detail::DrawElementListCollector* elementListManager);

	// TODO: internal
	detail::CameraInfo cameraInfo;

protected:
	// アタッチされている stageList を実行し、frameBuffer へレンダリングを行う。
	void render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer);

LN_INTERNAL_ACCESS:
	const List<detail::DrawElementListCollector*>& elementListManagers() const { return m_elementListManagers; }
	const Ref<detail::FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }

private:
	detail::RenderingManager* m_manager;
	Ref<detail::FrameBufferCache> m_frameBufferCache;
	List<detail::DrawElementListCollector*> m_elementListManagers;
};

} // namespace ln


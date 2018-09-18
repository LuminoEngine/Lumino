#pragma once
#include "../Graphics/Common.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/DepthBuffer.hpp"
#include "../Shader/ShaderInterfaceFramework.hpp"

namespace ln {
class GraphicsContext;
namespace detail {
class DrawElementList;
class DrawElementListManager;
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
	// World の描画などでは RenderView のインスタンスを持ち回り、描画してほしい人が Manager を ここに add していく。
	void clearDrawElementListManagers();
	void addDrawElementListManager(detail::DrawElementListManager* elementListManager);

	// TODO: internal
	detail::CameraInfo cameraInfo;

protected:
	// アタッチされている stageList を実行し、frameBuffer へレンダリングを行う。
	void render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer);

LN_INTERNAL_ACCESS:
	const List<detail::DrawElementListManager*>& elementListManagers() const { return m_elementListManagers; }

private:
	List<detail::DrawElementListManager*> m_elementListManagers;
};

} // namespace ln


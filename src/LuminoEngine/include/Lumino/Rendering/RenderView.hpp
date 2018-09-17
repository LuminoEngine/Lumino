#pragma once
#include "../Graphics/Common.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/DepthBuffer.hpp"
#include "../Shader/ShaderInterfaceFramework.hpp"

namespace ln {
class GraphicsContext;
namespace detail {
class DrawElementList;
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
	void attachDrawElementList(detail::DrawElementList* elementList);

	// TODO: internal
	detail::CameraInfo cameraInfo;

protected:
	// アタッチされている stageList を実行し、frameBuffer へレンダリングを行う。
	void render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer);

LN_INTERNAL_ACCESS:
	const List<detail::DrawElementList*>& elementLists() const { return m_elementLists; }

private:
	List<detail::DrawElementList*> m_elementLists;
};

} // namespace ln


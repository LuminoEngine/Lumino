#pragma once
#include "../Graphics/Texture.hpp"
#include "../Graphics/DepthBuffer.hpp"

namespace ln {
namespace detail {
class DrawElementList;
}

struct FrameBuffer
{
	Ref<RenderTargetTexture> renderTarget;
	Ref<DepthBuffer> depthBuffer;
};

class RenderView
	: public Object
{
public:

protected:
	// 構築済みの stageList を実行し、frameBuffer へレンダリングを行う。
	void render(const FrameBuffer& frameBuffer, detail::DrawElementList* stageList);

private:
};

} // namespace ln


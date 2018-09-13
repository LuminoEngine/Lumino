#pragma once
#include "../Graphics/Texture.hpp"
#include "../Graphics/DepthBuffer.hpp"

namespace ln {
namespace detail {
class RenderStageList;
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
	// TODO: internal
	void render(const FrameBuffer& frameBuffer, detail::RenderStageList* stageList);

private:
};

} // namespace ln


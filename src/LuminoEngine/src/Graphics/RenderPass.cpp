/*
	移行 Note:
		map を各 GraphicsResource から CommandBuffer へ移動する。
		※ ネイティブの map (transfar) は RenderPass の外側でなければ使えないので、順序制御するため CommandBuffer に統合したい

		CommandBuffer と RenderPass は全て pool からインスタンスを得る。インスタンスを外側で保持し続けてはならない。
*/
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>

namespace ln {

//==============================================================================
// RenderPass

RenderPass::RenderPass()
    : m_rhiObject()
{
}

RenderPass::~RenderPass()
{
}

void RenderPass::init()
{
    GraphicsResource::init();
}

void RenderPass::onDispose(bool explicitDisposing)
{
    m_rhiObject = nullptr;
    GraphicsResource::onDispose(explicitDisposing);
}

void RenderPass::onChangeDevice(detail::IGraphicsDevice* device)
{
}

detail::IRenderPass* RenderPass::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	return nullptr;
}

} // namespace ln

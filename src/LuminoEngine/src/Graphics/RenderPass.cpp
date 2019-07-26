
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


#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsResource.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

GraphicsResource::GraphicsResource()
	: m_manager(nullptr)
{

}

GraphicsResource::~GraphicsResource()
{

}

void GraphicsResource::initialize()
{
	m_manager = detail::EngineDomain::graphicsManager();
}

detail::IGraphicsDeviceContext* GraphicsResource::deviceContext() const
{
	return m_manager->deviceContext();
}

} // namespace ln

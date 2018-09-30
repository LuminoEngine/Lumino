
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsResource.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// GraphicsResource

GraphicsResource::GraphicsResource()
	: m_manager(nullptr)
{

}

GraphicsResource::~GraphicsResource()
{

}

void GraphicsResource::initialize()
{
	if (LN_REQUIRE(!m_manager)) return;
	m_manager = detail::EngineDomain::graphicsManager();
	m_manager->addGraphicsResource(this);
}

void GraphicsResource::dispose()
{
	if (m_manager) {
		m_manager->removeGraphicsResource(this);
		m_manager = nullptr;
	}
	Object::dispose();
}

detail::IGraphicsDeviceContext* GraphicsResource::deviceContext() const
{
	return m_manager->deviceContext();
}

} // namespace ln

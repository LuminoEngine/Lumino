
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsResource.hpp>

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

} // namespace ln

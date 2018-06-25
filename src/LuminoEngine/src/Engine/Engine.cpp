
#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <Lumino/Engine/Engine.hpp>
#include <Lumino/Graphics/GraphicsContext.hpp>
#include "../Graphics/GraphicsManager.hpp"

namespace ln {

//==============================================================================
// Engine

void Engine::initialize()
{
	detail::EngineDomain::engineManager()->initialize();
}

void Engine::terminate()
{
	detail::EngineDomain::release();
}

bool Engine::update()
{
	return detail::EngineDomain::engineManager()->updateUnitily();
}

GraphicsContext* Engine::graphicsContext()
{
	return detail::EngineDomain::graphicsManager()->graphicsContext();
}

} // namespace ln


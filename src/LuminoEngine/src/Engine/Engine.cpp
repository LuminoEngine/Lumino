
#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <Lumino/Engine/Engine.hpp>
#include <Lumino/Graphics/GraphicsContext.hpp>
#include "../Graphics/GraphicsManager.hpp"

namespace ln {

//==============================================================================
// Engine

static void beginFrame()
{
	detail::EngineDomain::engineManager()->updateFrame();
	detail::EngineDomain::engineManager()->renderFrame();
}

static void endFrame()
{
	detail::EngineDomain::engineManager()->presentFrame();
}

void Engine::initialize()
{
	detail::EngineDomain::engineManager()->initialize();
	beginFrame();
}

void Engine::terminate()
{
	endFrame();
	detail::EngineDomain::release();
}

bool Engine::update()
{
	endFrame();
	beginFrame();
	return !detail::EngineDomain::engineManager()->isExitRequested();
}

GraphicsContext* Engine::graphicsContext()
{
	return detail::EngineDomain::graphicsManager()->graphicsContext();
}

} // namespace ln



#include "Internal.hpp"
#include "EngineManager.hpp"
//#include "EngineDomain.hpp"
#include <Lumino/Engine/Engine.hpp>

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

} // namespace ln


﻿
#include "Internal.hpp"
#include "EngineManager.hpp"
#include <LuminoEngine/Engine/Time.hpp>

namespace ln {

//==============================================================================
// Time

float Time::deltaTime()
{
	return detail::EngineDomain::engineManager()->fpsController().getElapsedGameTime();
}

} // namespace ln


﻿#include "Internal.hpp"
#include <LuminoGraphics/RHIModule.hpp>
#include "GraphicsManager.hpp"

namespace ln {

RHIModule* RHIModule::initialize(const RHIModuleSettings& settings) {
    detail::GraphicsManager::Settings s = settings;
    return detail::GraphicsManager::initialize(s);
}

void RHIModule::terminate() {
    detail::GraphicsManager::terminate();
}

} // namespace ln
#include "Internal.hpp"
#include <LuminoPlatform/PlatformApplication.hpp>
#include "PlatformManager.hpp"

namespace ln {

//==============================================================================
// PlatformApplication

	
PlatformApplication::PlatformApplication() {
    m_manager = static_cast<detail::PlatformManager*>(PlatformModule::initialize());


}

int PlatformApplication::run() {
	return 0;
}

void PlatformApplication::onFrameBegin() {
    m_manager->processSystemEventQueue();
}

} // namespace ln

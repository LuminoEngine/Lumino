#include "Internal.hpp"
#include <LuminoEngine/Engine/CoreApplication.hpp>

namespace ln {

//==============================================================================
// CoreApplication

CoreApplication* CoreApplication::s_instance = nullptr;

CoreApplication::CoreApplication() {
    if (LN_ASSERT(!s_instance)) return;

    s_instance = this;
}

CoreApplication::~CoreApplication() {
    if (s_instance == this) {
        s_instance = nullptr;
    }
}
	
} // namespace ln

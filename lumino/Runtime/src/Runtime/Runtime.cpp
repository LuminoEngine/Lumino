#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Runtime/Runtime.hpp>

namespace ln {

//==============================================================================
// Runtime

Result Runtime::initialize() {
    if (EngineContext2::initialize())
        return ok();
    else
        return err();
}

void Runtime::terminate() {
    EngineContext2::terminate();
}

} // namespace ln

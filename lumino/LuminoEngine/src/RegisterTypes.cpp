#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine.hpp>

namespace ln {

void registerTypes_Engine(EngineContext2* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

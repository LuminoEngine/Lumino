#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine.hpp>

namespace ln {

void registerTypes_Engine(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

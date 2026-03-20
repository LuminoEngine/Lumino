#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoFramework.hpp>

namespace ln {

void registerTypes_Engine(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

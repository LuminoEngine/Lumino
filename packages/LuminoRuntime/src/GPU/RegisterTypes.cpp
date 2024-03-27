#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/Shader.hpp>

namespace ln {

void registerTypes_Graphics(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

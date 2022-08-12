#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/Shader.hpp>

namespace ln {

void registerTypes_Graphics(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

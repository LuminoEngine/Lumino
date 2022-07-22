#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/Shader.hpp>

namespace ln {

void registerTypes_Graphics(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

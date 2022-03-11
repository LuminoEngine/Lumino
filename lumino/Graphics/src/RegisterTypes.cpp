#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/Shader.hpp>

namespace ln {

void registerTypes_Graphics(EngineContext2* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

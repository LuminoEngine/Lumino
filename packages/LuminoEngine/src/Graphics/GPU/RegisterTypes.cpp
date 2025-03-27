#include <LuminoEngine/Engine/EngineInstance.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/Shader.hpp>

namespace ln {

void registerTypes_Graphics(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln

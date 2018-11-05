
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/ToneImageEffect.hpp>

namespace ln {

//==============================================================================
// ToneImageEffect

ToneImageEffect::ToneImageEffect()
{
}

ToneImageEffect::~ToneImageEffect()
{
}

void ToneImageEffect::initialize()
{
    ImageEffect::initialize();
    auto shader = newObject<Shader>(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Rendering/Resource/ToneImageEffect.fx");

    m_material = newObject<Material>();
    m_material->setShader(shader);
}

void ToneImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source); // TODO: blit が source 指定未対応なので
    context->blit(nullptr, destination, m_material);
}

} // namespace ln


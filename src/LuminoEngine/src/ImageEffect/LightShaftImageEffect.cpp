
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/ImageEffect/LightShaftImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// LightShaftImageEffect

//Ref<LightShaftImageEffect> LightShaftImageEffect::create()
//{
//    return makeObject<LightShaftImageEffect>();
//}
//
//LightShaftImageEffect::LightShaftImageEffect()
//{
//}
//
//void LightShaftImageEffect::init()
//{
//    ImageEffect::init();
//	m_radialBlur = makeObject<Material>();
//	m_radialBlur->setShader(detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::RadialBlur));
//}
//
//void LightShaftImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//	m_radialBlur->setMainTexture(source);
//
//	// TODO: Add だと黒っぽいボリュームシャドウに見える部分が見えなくなる。代わりにすごく明るくなるけど。
//	// Add しない場合は、Shader 側で LightOcclusionMap と source を合成する必要がある。
//	//context->setBlendMode(BlendMode::Add);
//
//    context->blit(m_radialBlur, destination);
//}

} // namespace ln


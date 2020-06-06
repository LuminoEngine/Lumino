
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/ImageEffect/SSRImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"
//
//#include <LuminoEngine/Bitmap/Texture.hpp>
//#include <LuminoEngine/Input/Input.hpp>
namespace ln {
    extern Texture* g_viewNormalMap;
    extern Texture* g_viewMaterialMap;
    Texture* g_srTarget = nullptr;

//==============================================================================
// SSRImageEffect

SSRImageEffect::SSRImageEffect()
{
}

void SSRImageEffect::init()
{
    ImageEffect::init();
}

Ref<ImageEffectInstance> SSRImageEffect::onCreateInstance()
{
    return makeObject<detail::SSRImageEffectInstance>(this);
}

//==============================================================================
// SSRImageEffectInstance

namespace detail {

SSRImageEffectInstance::SSRImageEffectInstance()
    : m_owner(nullptr)
    , m_viewWidth(0)
    , m_viewHeight(0)
{
}

bool SSRImageEffectInstance::init(SSRImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    m_owner = owner;


    auto shader1 = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/SSR.fx");
    m_ssrMaterial = makeObject<Material>();
    m_ssrMaterial->setShader(shader1);

    auto shader2 = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/SSRBlur.fx");
    m_ssrBlurMaterial1 = makeObject<Material>();
    m_ssrBlurMaterial1->setShader(shader2);
    m_ssrBlurMaterial2 = makeObject<Material>();
    m_ssrBlurMaterial2->setShader(shader2);

    auto shader3 = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/SSRComposite.fx");
    m_ssrCompositeMaterial = makeObject<Material>();
    m_ssrCompositeMaterial->setShader(shader3);

    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

void SSRImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    int resx = source->width();
    int resy = source->height();
    if (m_viewWidth != resx || m_viewHeight != resy) {
        resetResources(resx, resy);
    }


    //// Composite All the mips
    //m_compositeMaterial->setMainTexture(source);
    //Vector4 bloomTintColors[] = { Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1) };
    //m_compositeMaterial->setFloat(u"_SSRStrength", m_owner->m_bloomStrength);
    //m_compositeMaterial->setFloat(u"_SSRRadius", m_owner->m_bloomRadius);
    //m_compositeMaterial->setVectorArray(u"_SSRTintColors", bloomTintColors, MIPS);

    //auto proj = Matrix::makePerspectiveFovRH(context->viewPoint()->fovY, 640.0 / 480.0, 0.3, 1000);
    auto proj = context->viewPoint()->projMatrix;
    //auto tproj1 = Matrix::makeTranspose(proj);
    auto tproj1 = proj;
    auto tproj2 = Matrix::makeInverse(proj);
    //auto tproj2 = Matrix::makeTranspose(Matrix::makeInverse(proj));
    //auto proj = context->viewPoint()->projMatrix;

    m_ssrMaterial->setTexture(u"_ColorSampler", source);
    m_ssrMaterial->setTexture(u"_NormalAndDepthSampler", g_viewNormalMap);
    m_ssrMaterial->setTexture(u"_MetalRoughSampler", g_viewMaterialMap);
    m_ssrMaterial->setFloat(u"_Projection23", proj(2, 3));
    m_ssrMaterial->setFloat(u"_Projection33", proj(3, 3));
    m_ssrMaterial->setMatrix(u"_CameraProjectionMatrix", tproj1);
    m_ssrMaterial->setMatrix(u"_CameraInverseProjectionMatrix", tproj2);
    context->blit(m_ssrMaterial, m_ssrTarget);
    g_srTarget = m_ssrTarget;

#if 0
    context->blit(m_ssrMaterial, destination);
#else
    m_ssrBlurMaterial1->setTexture(u"_ColorSampler", m_ssrTarget);
    context->blit(m_ssrBlurMaterial1, m_blurTarget1);

    m_ssrBlurMaterial2->setTexture(u"_ColorSampler", m_blurTarget1);
    context->blit(m_ssrBlurMaterial2, m_blurTarget2);

    m_ssrCompositeMaterial->setTexture(u"_ColorSampler", source);
    m_ssrCompositeMaterial->setTexture(u"_SSRSampler", m_ssrTarget);
    context->blit(m_ssrCompositeMaterial, destination);
#endif

    //context->blit(m_ssrMaterial, destination);

    //if (Input::triggered("submit")) {
    //    detail::TextureInternal::readData(dynamic_cast<ln::RenderTargetTexture*>(g_viewMaterialMap), context->gra)->save(u"test.png");
    //}
}

void SSRImageEffectInstance::resetResources(int resx, int resy)
{
    //m_renderTargetsHorizontal.clear();
    //m_renderTargetsVertical.clear();

    // TODO: tempolary からとっていいかも
    m_ssrTarget = makeObject<RenderTargetTexture>(resx, resy, TextureFormat::RGBA8, false);
    m_ssrTarget->setSamplerState(m_samplerState);
    m_blurTarget1 = makeObject<RenderTargetTexture>(resx, resy, TextureFormat::RGBA8, false);
    m_blurTarget1->setSamplerState(m_samplerState);
    m_blurTarget2 = makeObject<RenderTargetTexture>(resx, resy, TextureFormat::RGBA8, false);
    m_blurTarget2->setSamplerState(m_samplerState);
    m_compositeTarget = makeObject<RenderTargetTexture>(resx, resy, TextureFormat::RGBA8, false);
    m_compositeTarget->setSamplerState(m_samplerState);

    //// Create render targets for down sampling.
    //auto rx = std::round(resx / 2);
    //auto ry = std::round(resy / 2);
    //for (int i = 0; i < MIPS; i++) {
    //    auto renderTargetHorizonal = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false);
    //    renderTargetHorizonal->setSamplerState(m_samplerState);
    //    m_renderTargetsHorizontal.add(renderTargetHorizonal);

    //    auto renderTargetVertical = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false);
    //    renderTargetVertical->setSamplerState(m_samplerState);
    //    m_renderTargetsVertical.add(renderTargetVertical);

    //    rx = std::round(resx / 2);
    //    ry = std::round(resy / 2);
    //}

    //// Composite material
    //float bloomFactors[] = { 1.0, 0.8, 0.6, 0.4, 0.2 };
    //m_compositeMaterial->setTexture(u"_BlurTexture1", m_renderTargetsVertical[0]);
    //m_compositeMaterial->setTexture(u"_BlurTexture2", m_renderTargetsVertical[1]);
    //m_compositeMaterial->setTexture(u"_BlurTexture3", m_renderTargetsVertical[2]);
    //m_compositeMaterial->setTexture(u"_BlurTexture4", m_renderTargetsVertical[3]);
    //m_compositeMaterial->setTexture(u"_BlurTexture5", m_renderTargetsVertical[4]);
    //m_compositeMaterial->setFloatArray(u"_SSRFactors", bloomFactors, MIPS);

    m_viewWidth = resx;
    m_viewHeight = resy;
}

} // namespace detail
} // namespace ln


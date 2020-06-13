
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/ImageEffect/TransitionImageEffect.hpp>

namespace ln {

//==============================================================================
// TransitionImageEffect

Ref<TransitionImageEffect> TransitionImageEffect::create()
{
    return makeObject<TransitionImageEffect>();
}

TransitionImageEffect::TransitionImageEffect()
    : m_mode(Mode::FadeOut)
    , m_fadeColor(Color::Black)
    , m_factor()
    , m_vague(0.2)
    , m_freezeRevision(0)
{
}

void TransitionImageEffect::init()
{
    ImageEffect::init();
}

void TransitionImageEffect::setMaskTexture(Texture* value)
{
    m_maskTexture = value;
}

Texture* TransitionImageEffect::maskTexture() const
{
    return m_maskTexture;
}

void TransitionImageEffect::startFadeOut(float duration)
{
    m_mode = Mode::FadeOut;
    m_factor.start(1.0f, duration);
    m_freezeRevision++;
}

void TransitionImageEffect::startFadeIn(float duration)
{
    m_mode = Mode::FadeIn;
    m_factor.start(0.0f,  duration);
    m_freezeRevision++;
}

void TransitionImageEffect::startCrossFade(float duration)
{
    m_mode = Mode::CrossFade;
    m_factor.start(1.0f, 0.0f, duration);
    m_freezeRevision++;
}

void TransitionImageEffect::onUpdateFrame(float elapsedSeconds)
{
    m_factor.advanceTime(elapsedSeconds);
}

Ref<ImageEffectInstance> TransitionImageEffect::onCreateInstance()
{
    return makeObject<detail::TransitionImageEffectInstance>(this);
}

//
//void TransitionImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//}
//virtual void onPostRender() override;

namespace detail {

TransitionImageEffectInstance::TransitionImageEffectInstance()
    : m_freezeRevision(0)
{

}

bool TransitionImageEffectInstance::init(TransitionImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    m_owner = owner;

    m_withoutMaskMaterial = makeObject<Material>();
    m_withoutMaskMaterial->setShader(makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/TransitionImageEffectWithoutMask.fx"));

    m_withMaskMaterial = makeObject<Material>();
    m_withMaskMaterial->setShader(makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/TransitionImageEffectWithMask.fx"));
    
    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->shadingModel = ShadingModel::Unlit;

    return true;
}

bool TransitionImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    bool recreated = preparePreviousFrameTarget(source->width(), source->height());
    if (m_freezeRevision != m_owner->m_freezeRevision) {
        if (recreated) {
            // 起動直後など、レンダリングターゲット作成後1回も描画していないのに遷移しようとしている。
            // このときはビューの背景色を使う。

            // TODO: scoped または blit みたいに RT 直接指定の Clear
            Ref<RenderTargetTexture> oldTarget = context->renderTarget(0);
            context->setRenderTarget(0, m_overrayTarget);
            context->clear(ClearFlags::Color, context->baseRenderView->backgroundColor());
            context->setRenderTarget(0, oldTarget);
        }
        else {
            std::swap(m_previousFrameTarget, m_overrayTarget);
        }

        m_freezeRevision = m_owner->m_freezeRevision;
    }


   // if (!m_owner->isRunning()) {
    if (Math::nearEqual(m_owner->m_factor.value(), 0.0f)) {
        // No effect apply.
        // destination への転送は行わないが、prev として保存はしておく
        m_copyMaterial->setMainTexture(source);
        //context->blit(m_copyMaterial, destination);
        context->blit(m_copyMaterial, m_previousFrameTarget);
        return false;
    }
    else {
        if (m_maskTexture != m_owner->m_maskTexture) {
            m_maskTexture = m_owner->m_maskTexture;
            m_withMaskMaterial->setTexture(u"_MaskTexture", m_maskTexture);
        }

        if (m_owner->m_mode == TransitionImageEffect::Mode::FadeIn || m_owner->m_mode == TransitionImageEffect::Mode::FadeOut) {
            renderFadeInOut(context, source, destination, m_owner->m_factor.value());
        }
        else if (m_owner->m_mode == TransitionImageEffect::Mode::CrossFade) {
            renderCrossFade(context, source, destination);
        }
        return true;
    }

}

bool TransitionImageEffectInstance::preparePreviousFrameTarget(int width, int height)
{
    if (!m_overrayTarget) {
        // こちらは初回だけ適当なサイズで作ればOK.
        // freeze のたびに m_previousFrameTarget と swap されるので、その際にサイズが足りなければ ↓ の
        // m_previousFrameTarget の再構築処理で再構築される。
        m_overrayTarget = makeObject<RenderTargetTexture>(width, height, TextureFormat::RGBA8, false);
    }

    if (!m_previousFrameTarget || (m_previousFrameTarget->width() != width || m_previousFrameTarget->height() != height)) {
        m_previousFrameTarget = makeObject<RenderTargetTexture>(width, height, TextureFormat::RGBA8, false);
        return true;
    }
    else {
        return false;
    }
}

void TransitionImageEffectInstance::renderFadeInOut(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination, float factor)
{
    context->setDepthTestEnabled(false);
    context->setDepthWriteEnabled(false);

    //bool recreated = preparePreviousFrameTarget(source->width(), source->height());
    //if (m_freezeRequested || recreated) {
    //    // TODO: scoped または blit みたいに RT 直接指定の Clear
    //    Ref<RenderTargetTexture> oldTarget = context->renderTarget(0);
    //    context->setRenderTarget(0, m_previousFrameTarget);
    //    context->clear(ClearFlags::Color, context->baseRenderView->backgroundColor());
    //    context->setRenderTarget(0, oldTarget);
    //    m_freezeRequested = false;
    //}

    //std::cout << m_owner->m_factor.value() << std::endl;

    Material* material = nullptr;
    if (m_owner->m_maskTexture)
    {
        m_withMaskMaterial->setFloat(u"_Factor", m_owner->m_factor.value());
        m_withMaskMaterial->setVector(u"_ColorScale", m_owner->m_fadeColor.toVector4());
        m_withMaskMaterial->setFloat(u"_Vague", m_owner->m_vague);
        m_withMaskMaterial->setTexture(u"_OverrayTexture", Texture2D::whiteTexture());
        material = m_withMaskMaterial;
    }
    else
    {
        m_withoutMaskMaterial->setFloat(u"_Factor", factor);
        m_withoutMaskMaterial->setVector(u"_ColorScale", m_owner->m_fadeColor.toVector4());
        m_withoutMaskMaterial->setFloat(u"_Vague", m_owner->m_vague);
        m_withoutMaskMaterial->setTexture(u"_OverrayTexture", Texture2D::whiteTexture());
        material = m_withoutMaskMaterial;
    }

    // エフェクト適用済みの描画結果を前回の描画結果として毎回覚えておく
    material->setMainTexture(source);
    context->blit(material, m_previousFrameTarget);

    // Final output.
    m_copyMaterial->setMainTexture(m_previousFrameTarget);
    context->blit(m_copyMaterial, destination);

    //// Final output.
    //material->setMainTexture(source);
    //context->blit(material, destination);


    //m_copyMaterial->setMainTexture(source);
    //context->blit(m_copyMaterial, destination);
}

void TransitionImageEffectInstance::renderCrossFade(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{


    Material* material = nullptr;
    if (m_owner->m_maskTexture)
    {
        m_withMaskMaterial->setFloat(u"_Factor", m_owner->m_factor.value());
        m_withMaskMaterial->setVector(u"_ColorScale", Vector4(1, 1, 1, 1));
        m_withMaskMaterial->setFloat(u"_Vague", m_owner->m_vague);
        m_withMaskMaterial->setTexture(u"_OverrayTexture", m_overrayTarget);
        material = m_withMaskMaterial;
    }
    else
    {
        m_withoutMaskMaterial->setFloat(u"_Factor", m_owner->m_factor.value());
        m_withoutMaskMaterial->setVector(u"_ColorScale", Vector4(1, 1, 1, 1));
        m_withoutMaskMaterial->setFloat(u"_Vague", m_owner->m_vague);
        m_withoutMaskMaterial->setTexture(u"_OverrayTexture", m_overrayTarget);
        material = m_withoutMaskMaterial;
    }

    // エフェクト適用済みの描画結果を前回の描画結果として毎回覚えておく
    material->setMainTexture(source);
    context->blit(material, m_previousFrameTarget);

    // Final output.
    m_copyMaterial->setMainTexture(m_previousFrameTarget);
    context->blit(m_copyMaterial, destination);
}

} // namespace detail
} // namespace ln


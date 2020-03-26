
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
    , m_freezeRequested(false)
    , m_factor()
{
}

void TransitionImageEffect::init()
{
    ImageEffect::init();
    auto shader = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/TransitionImageEffectWithoutMask.fx");

    m_withoutMaskMaterial = makeObject<Material>();
    m_withoutMaskMaterial->setShader(shader);

    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->shadingModel = ShadingModel::Unlit;
}

void TransitionImageEffect::startFadeOut(float duration)
{
    m_mode = Mode::FadeOut;
    m_factor.start(0.0f, 1.0f, duration);
    m_freezeRequested = true;
}

void TransitionImageEffect::startFadeIn(float duration)
{
    m_mode = Mode::FadeIn;
    m_factor.start(1.0f, 0.0f,  duration);
    m_freezeRequested = true;
}

void TransitionImageEffect::startCrossFade(float duration, int vague)
{
    m_mode = Mode::CrossFade;
    m_factor.start(0.0f, duration);
    m_freezeRequested = true;
}

void TransitionImageEffect::onUpdateFrame(float elapsedSeconds)
{
    m_factor.advanceTime(elapsedSeconds);
}

void TransitionImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    if (!isRunning()) {
        // No effect apply.
        m_copyMaterial->setMainTexture(source);
        context->blit(m_copyMaterial, destination);
    }
    else if (m_mode == Mode::FadeIn || m_mode == Mode::FadeOut) {
        renderFadeInOut(context, source, destination, m_factor.value());
    }
    else if (m_mode == Mode::CrossFade) {
        renderCrossFade(context, source, destination);
    }
}

bool TransitionImageEffect::preparePreviousFrameTarget(int width, int height)
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

void TransitionImageEffect::renderFadeInOut(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination, float factor)
{
    //bool recreated = preparePreviousFrameTarget(source->width(), source->height());
    //if (m_freezeRequested || recreated) {
    //    // TODO: scoped または blit みたいに RT 直接指定の Clear
    //    Ref<RenderTargetTexture> oldTarget = context->renderTarget(0);
    //    context->setRenderTarget(0, m_previousFrameTarget);
    //    context->clear(ClearFlags::Color, context->baseRenderView->backgroundColor());
    //    context->setRenderTarget(0, oldTarget);
    //    m_freezeRequested = false;
    //}

    Material* material = nullptr;
    if (0)
    {
        // マスクテクスチャ使用
        LN_NOTIMPLEMENTED();
    }
    else
    {
        // マスクテクスチャ不使用
        m_withoutMaskMaterial->setFloat(u"_Factor", factor);
        m_withoutMaskMaterial->setVector(u"_ColorScale", Vector4(0, 0, 0, 1));
        m_withoutMaskMaterial->setTexture(u"_OverrayTexture", Texture2D::whiteTexture());
        material = m_withoutMaskMaterial;
    }

    // Final output.
    material->setMainTexture(source);
    context->blit(material, destination);
}

void TransitionImageEffect::renderCrossFade(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    bool recreated = preparePreviousFrameTarget(source->width(), source->height());
    if (m_freezeRequested) {
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

        m_freezeRequested = false;
    }


    Material* material = nullptr;
    if (0)
    {
        // マスクテクスチャ使用
        LN_NOTIMPLEMENTED();
    }
    else
    {
        // マスクテクスチャ不使用
        m_withoutMaskMaterial->setFloat(u"_Factor", m_factor.value());
        m_withoutMaskMaterial->setVector(u"_ColorScale", Vector4(1, 1, 1, 1));
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

} // namespace ln


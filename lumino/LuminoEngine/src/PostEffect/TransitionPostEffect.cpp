#include "Internal.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/RenderPass.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/TransitionPostEffect.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {

//==============================================================================
// TransitionPostEffect

Ref<TransitionPostEffect> TransitionPostEffect::create()
{
    return makeObject<TransitionPostEffect>();
}

TransitionPostEffect::TransitionPostEffect()
    : m_mode(Mode::FadeOut)
    , m_fadeColor(Color::Black)
    , m_factor()
    , m_vague(0.2)
    , m_freezeRevision(0)
{
}

void TransitionPostEffect::init()
{
    PostEffect::init();
}

void TransitionPostEffect::setMaskTexture(Texture* value)
{
    m_maskTexture = value;
}

Texture* TransitionPostEffect::maskTexture() const
{
    return m_maskTexture;
}

void TransitionPostEffect::startFadeOut(float duration)
{
    m_mode = Mode::FadeOut;
    m_factor.start(1.0f, duration);
    m_freezeRevision++;
}

void TransitionPostEffect::startFadeIn(float duration)
{
    m_mode = Mode::FadeIn;
    m_factor.start(0.0f,  duration);
    m_freezeRevision++;
}

void TransitionPostEffect::startCrossFade(float duration)
{
    m_mode = Mode::CrossFade;
    m_factor.start(1.0f, 0.0f, duration);
    m_freezeRevision++;
}

void TransitionPostEffect::onUpdateFrame(float elapsedSeconds)
{
    m_factor.advanceTime(elapsedSeconds);
}

Ref<PostEffectInstance> TransitionPostEffect::onCreateInstance()
{
    return makeObject<detail::TransitionPostEffectInstance>(this);
}

//
//void TransitionPostEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//}
//virtual void onPostRender() override;

namespace detail {

TransitionPostEffectInstance::TransitionPostEffectInstance()
    : m_freezeRevision(0)
{

}

bool TransitionPostEffectInstance::init(TransitionPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    m_owner = owner;

    m_withoutMaskMaterial = makeObject<Material>();
    m_withoutMaskMaterial->setShader(RenderingManager::instance()->builtinShader(BuiltinShader::TransitionEffectWithoutMask));

    m_withMaskMaterial = makeObject<Material>();
    m_withMaskMaterial->setShader(RenderingManager::instance()->builtinShader(BuiltinShader::TransitionEffectWithMask));
    
    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->setShader(RenderingManager::instance()->builtinShader(BuiltinShader::CopyScreen));
    //m_copyMaterial->setShadingModel(ShadingModel::Unlit);

    return true;
}

bool TransitionPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    bool recreated = preparePreviousFrameTarget(source->width(), source->height());
    if (m_freezeRevision != m_owner->m_freezeRevision) {
        if (recreated) {
            // 起動直後など、レンダリングターゲット作成後1回も描画していないのに遷移しようとしている。
            // このときはビューの背景色を使う。

            // TODO: scoped または blit みたいに RT 直接指定の Clear
            auto* oldTarget = context->renderPass();
            auto* newTarget = RenderPass::get(m_overrayTarget, nullptr);
            context->setRenderPass(oldTarget);
            context->clear(ClearFlags::Color, renderView->backgroundColor());
            context->setRenderPass(oldTarget);
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
            m_withMaskMaterial->setTexture(_TT("_MaskTexture"), m_maskTexture);
        }

        if (m_owner->m_mode == TransitionPostEffect::Mode::FadeIn || m_owner->m_mode == TransitionPostEffect::Mode::FadeOut) {
            renderFadeInOut(context, source, destination, m_owner->m_factor.value());
        }
        else if (m_owner->m_mode == TransitionPostEffect::Mode::CrossFade) {
            renderCrossFade(context, source, destination);
        }
        return true;
    }

}

bool TransitionPostEffectInstance::preparePreviousFrameTarget(int width, int height)
{
    if (!m_overrayTarget) {
        // こちらは初回だけ適当なサイズで作ればOK.
        // freeze のたびに m_previousFrameTarget と swap されるので、その際にサイズが足りなければ ↓ の
        // m_previousFrameTarget の再構築処理で再構築される。
        m_overrayTarget = makeObject<RenderTargetTexture>(width, height, TextureFormat::RGBA8, false, false);
    }

    if (!m_previousFrameTarget || (m_previousFrameTarget->width() != width || m_previousFrameTarget->height() != height)) {
        m_previousFrameTarget = makeObject<RenderTargetTexture>(width, height, TextureFormat::RGBA8, false, false);
        return true;
    }
    else {
        return false;
    }
}

void TransitionPostEffectInstance::renderFadeInOut(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination, float factor)
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
        m_withMaskMaterial->setFloat(_TT("_Factor"), m_owner->m_factor.value());
        m_withMaskMaterial->setVector(_TT("_ColorScale"), m_owner->m_fadeColor.toVector4());
        m_withMaskMaterial->setFloat(_TT("_Vague"), m_owner->m_vague);
        m_withMaskMaterial->setTexture(_TT("_OverrayTexture"), Texture2D::whiteTexture());
        material = m_withMaskMaterial;
    }
    else
    {
        m_withoutMaskMaterial->setFloat(_TT("_Factor"), factor);
        m_withoutMaskMaterial->setVector(_TT("_ColorScale"), m_owner->m_fadeColor.toVector4());
        m_withoutMaskMaterial->setFloat(_TT("_Vague"), m_owner->m_vague);
        m_withoutMaskMaterial->setTexture(_TT("_OverrayTexture"), Texture2D::whiteTexture());
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

void TransitionPostEffectInstance::renderCrossFade(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{


    Material* material = nullptr;
    if (m_owner->m_maskTexture)
    {
        m_withMaskMaterial->setFloat(_TT("_Factor"), m_owner->m_factor.value());
        m_withMaskMaterial->setVector(_TT("_ColorScale"), Vector4(1, 1, 1, 1));
        m_withMaskMaterial->setFloat(_TT("_Vague"), m_owner->m_vague);
        m_withMaskMaterial->setTexture(_TT("_OverrayTexture"), m_overrayTarget);
        material = m_withMaskMaterial;
    }
    else
    {
        m_withoutMaskMaterial->setFloat(_TT("_Factor"), m_owner->m_factor.value());
        m_withoutMaskMaterial->setVector(_TT("_ColorScale"), Vector4(1, 1, 1, 1));
        m_withoutMaskMaterial->setFloat(_TT("_Vague"), m_owner->m_vague);
        m_withoutMaskMaterial->setTexture(_TT("_OverrayTexture"), m_overrayTarget);
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


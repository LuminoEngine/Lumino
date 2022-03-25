
#include "Internal.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/PostEffect/ScreenBlurPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// ScreenBlurPostEffect

Ref<ScreenBlurPostEffect> ScreenBlurPostEffect::create()
{
    return makeObject<ScreenBlurPostEffect>();
}

ScreenBlurPostEffect::ScreenBlurPostEffect()
    : m_amountValue()
    //, m_accumTexture(nullptr)
    , m_center()
    , m_scale(1.0f)
{
}

ScreenBlurPostEffect::~ScreenBlurPostEffect()
{
}

void ScreenBlurPostEffect::init()
{
    PostEffect::init();
}

void ScreenBlurPostEffect::play(float amount, const Vector2& center, float scale, float duration)
{
    setRadialCenter(center);
    setRadialScale(scale);

    if (duration == 0.0f) {
        m_amountValue.start(amount, duration);
    }
    else {
        m_amountValue.start(amount, 0.0f, duration);
    }
}

void ScreenBlurPostEffect::onUpdateFrame(float elapsedSeconds)
{
    m_amountValue.advanceTime(elapsedSeconds);
}

Ref<PostEffectInstance> ScreenBlurPostEffect::onCreateInstance()
{
    return makeObject<detail::ScreenBlurPostEffectInstance>(this);
}


//void ScreenBlurPostEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//
//}

namespace detail {

ScreenBlurPostEffectInstance::ScreenBlurPostEffectInstance()
{
}

bool ScreenBlurPostEffectInstance::init(ScreenBlurPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    m_owner = owner;

    m_material = makeObject<Material>();
    //m_material->setShader(detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::ScreenBlurPostEffect));
    //m_material->setBlendMode(BlendMode::Alpha);
    auto shader = makeObject<Shader>(_TT("D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/PostEffect/Resource/ScreenBlur.fx"));
    m_material->setShader(shader);

    m_materialForCopySourceTo = makeObject<Material>();
    m_materialForCopyAccumTo = makeObject<Material>();

    return true;
}

bool ScreenBlurPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_materialForCopySourceTo->setMainTexture(source);

    float amount = m_owner->m_amountValue.value();
    if (amount <= 0.0f) {
        //context->blit(m_materialForCopySourceTo, destination);
        return false;
    }
    else {
        const auto& scale = m_owner->m_scale;
        const auto& center = m_owner->m_center;

        // m_accumTexture と source のサイズが異なる場合は作り直す
        if (m_accumTexture == nullptr || (m_accumTexture->width() != source->width() || m_accumTexture->height() != source->height()))
        {
            m_accumTexture = makeObject<RenderTargetTexture>(source->width(), source->height(), source->format(), false, false);
            context->blit(m_materialForCopySourceTo, m_accumTexture);
        }

        Matrix blurMatrix;
        blurMatrix.translate(-center.x, -center.y, 0);
        blurMatrix.scale(scale);
        blurMatrix.translate(center.x, center.y, 0);

        m_material->setVector(_TT("_BlurColor"), Vector4(1, 1, 1, amount));
        m_material->setMatrix(_TT("_BlurMatrix"), blurMatrix);

        //// m_accumTexture > source
        //context->blit(m_accumTexture, destination, m_material);

        //// save
        //context->blit(destination, m_accumTexture, Matrix::Identity);

        // m_accumTexture > source
        context->setBlendMode(BlendMode::Alpha);
        m_material->setMainTexture(m_accumTexture);
        context->blit(m_material, source);

        context->setBlendMode(BlendMode::Normal);

        // save
        context->blit(m_materialForCopySourceTo, m_accumTexture);

        m_materialForCopyAccumTo->setMainTexture(m_accumTexture);
        context->blit(m_materialForCopyAccumTo, destination);
        ////m_material->setMainTexture(source);
        ////context->blit(nullptr, destination, m_material);
        //context->blit(source, destination);

        return true;
    }
}

} // namespace detail
} // namespace ln


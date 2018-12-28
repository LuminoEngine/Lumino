
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/ImageEffect/ScreenBlurImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// ScreenBlurImageEffect

Ref<ScreenBlurImageEffect> ScreenBlurImageEffect::create()
{
    return newObject<ScreenBlurImageEffect>();
}

ScreenBlurImageEffect::ScreenBlurImageEffect()
    : m_material(nullptr)
    , m_amountValue()
    , m_accumTexture(nullptr)
    , m_center()
    , m_scale(1.0f)
{
}

ScreenBlurImageEffect::~ScreenBlurImageEffect()
{
}

void ScreenBlurImageEffect::initialize()
{
    ImageEffect::initialize();
    m_material = newObject<Material>();
    m_material->setShader(detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::ScreenBlurImageEffect));
    //m_material->setBlendMode(BlendMode::Alpha);
}

void ScreenBlurImageEffect::play(float amount, const Vector2& center, float scale, float duration)
{
    setRadialCenter(center);
    setRadialScale(scale);
    m_amountValue.start(amount, 0.0f, duration);
}

void ScreenBlurImageEffect::onUpdateFrame(float elapsedSeconds)
{
    m_amountValue.advanceTime(elapsedSeconds);
}

void ScreenBlurImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    float amount = m_amountValue.getValue();
    if (amount <= 0.0f) {
        context->blit(source, destination);
        return;
    }
    else {
        // m_accumTexture と source のサイズが異なる場合は作り直す
        if (m_accumTexture == nullptr || (m_accumTexture->width() != source->width() || m_accumTexture->height() != source->height()))
        {
            m_accumTexture = newObject<RenderTargetTexture>(source->width(), source->height(), source->format(), false);
            context->blit(source, m_accumTexture);
        }

        Matrix blurMatrix;
        blurMatrix.translate(-m_center.x, -m_center.y, 0);
        blurMatrix.scale(m_scale);
        blurMatrix.translate(m_center.x, m_center.y, 0);

        m_material->setVector(u"_BlurColor", Vector4(1, 1, 1, amount));
        m_material->setMatrix(u"_BlurMatrix", blurMatrix);

        //// m_accumTexture > source
        //context->blit(m_accumTexture, destination, m_material);

        //// save
        //context->blit(destination, m_accumTexture, Matrix::Identity);

        // m_accumTexture > source
        context->setBlendMode(BlendMode::Alpha);
        context->blit(m_accumTexture, source, m_material);

        context->setBlendMode(BlendMode::Normal);

        // save
        context->blit(source, m_accumTexture);

        context->blit(m_accumTexture, destination);
        ////m_material->setMainTexture(source);
        ////context->blit(nullptr, destination, m_material);
        //context->blit(source, destination);
    }

}

} // namespace ln


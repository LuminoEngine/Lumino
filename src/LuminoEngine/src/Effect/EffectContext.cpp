
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectContext.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>  // TODO:
#include <LuminoEngine/Rendering/Material.hpp>  // TODO:
#include "../Rendering/SpriteRenderFeature.hpp"  // TODO:
#include "../Engine/EngineManager.hpp"  // TODO:
#include <LuminoEngine/Scene/World.hpp>  // TODO:

namespace ln {

//==============================================================================
// EffectEmitter

EffectEmitter::EffectEmitter()
    : m_model(nullptr)
    , m_localTime(0.0f)
{
}

EffectEmitter::~EffectEmitter()
{
}

void EffectEmitter::init(EffectResource* model)
{
    Object::init();
    m_model = model;
}

bool EffectEmitter::update(float elapsedSeconds)
{
    m_localTime += elapsedSeconds;
    return onUpdate(m_localTime);
}

void EffectEmitter::render(RenderingContext* renderingContext)
{
    onRender(renderingContext);
}

bool EffectEmitter::onUpdate(float localTime)
{
    return false;
}

void EffectEmitter::onRender(RenderingContext* renderingContext)
{
}

//==============================================================================
// Effect

EffectEmitter* Effect::emit(EffectResource* effect, const Vector3& position)
{
    EffectEmitter* emitter = detail::EngineDomain::engineManager()->mainWorld()->effectContext()->createEmitter(effect);
    emitter->setPosition(position);
    return emitter;
}

//==============================================================================
// EffectContext

EffectContext::EffectContext()
    : m_manager(nullptr)
{
}

EffectContext::~EffectContext()
{
}

void EffectContext::init()
{
    Object::init();
}

EffectEmitter* EffectContext::createEmitter(EffectResource* model)
{
    // TODO: Pool

    // TODO: TypeEnum
    if (auto* sr = dynamic_cast<SpriteFrameEffectResource*>(model))
    {
        auto emitter = newObject<SpriteFrameEffectEmitter>(sr);
        m_emitters.add(emitter);
        return emitter;
    }

    return nullptr;
}

void EffectContext::update(float elapsedSeconds)
{
    for (auto itr = m_emitters.begin(); itr != m_emitters.end(); ) {
        if (!(*itr)->update(elapsedSeconds)) {
            itr = m_emitters.erase(itr);
        }
        else {
            ++itr;
        }
    }
    //for (auto& emitter : m_emitters) {
    //    emitter->update(elapsedSeconds);
    //}
}

void EffectContext::render(RenderingContext* renderingContext)
{
    for (auto& emitter : m_emitters) {
        emitter->render(renderingContext);
    }
}


//==============================================================================
// SpriteFrameEffectResource

SpriteFrameEffectResource::SpriteFrameEffectResource()
{
}

SpriteFrameEffectResource::~SpriteFrameEffectResource()
{
}

void SpriteFrameEffectResource::init(const Size& spriteSize, SpriteFrameSet* spriteFrameSet, int startNumber, int lastNumber, float frameTime)
{
    EffectResource::init();
    m_spriteSize = spriteSize;
    m_spriteFrameSet = spriteFrameSet;
    m_startNumber = startNumber;
    m_lastNumber = lastNumber;
    m_frameTime = frameTime;
    m_material = newObject<Material>();
    m_material->setMainTexture(m_spriteFrameSet->texture());
}

//==============================================================================
// SpriteFrameEffectEmitter

SpriteFrameEffectEmitter::SpriteFrameEffectEmitter()
{
}

SpriteFrameEffectEmitter::~SpriteFrameEffectEmitter()
{
}

void SpriteFrameEffectEmitter::init(SpriteFrameEffectResource* data)
{
    EffectEmitter::init(data);
    m_data = data;
    m_frameNumber = 0;
}

bool SpriteFrameEffectEmitter::onUpdate(float localTime)
{
    int totalFrameCount = m_data->m_lastNumber - m_data->m_startNumber + 1;

    if (localTime > m_data->m_frameTime * totalFrameCount) {
        return false;
    }

    m_frameNumber = m_data->m_startNumber + (localTime / m_data->m_frameTime);

    return true;
}

void SpriteFrameEffectEmitter::onRender(RenderingContext* renderingContext)
{
    if (SpriteFrame* frame = m_data->m_spriteFrameSet->frame(m_frameNumber)) {
        Rect sourceRect = frame->sourceRect();
        Vector2 anchorPoint = frame->anchorPoint();
        Texture* tex = m_data->m_spriteFrameSet->texture();


        anchorPoint.x = (Math::isNaN(anchorPoint.x)) ? 0.0f : anchorPoint.x;
        anchorPoint.y = (Math::isNaN(anchorPoint.y)) ? 0.0f : anchorPoint.y;

        Size renderSize;
        Rect renderSourceRect;
        detail::SpriteRenderFeature::makeRenderSizeAndSourceRectHelper(
            tex, m_data->m_spriteSize, sourceRect, &renderSize, &renderSourceRect);

        Matrix transform = Matrix::makeTranslation(position());

        renderingContext->drawSprite(
            transform, renderSize, anchorPoint, renderSourceRect, Color::White,
            SpriteBaseDirection::ZMinus, BillboardType::None, detail::SpriteFlipFlags::None, m_data->m_material);
    }
}

} // namespace ln

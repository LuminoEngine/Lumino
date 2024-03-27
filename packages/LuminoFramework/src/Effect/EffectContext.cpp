#include "Internal.hpp"
#include <LuminoFramework/Effect/EffectContext.hpp>
#include <LuminoFramework/Visual/SpriteComponent.hpp>  // TODO:
#include <LuminoEngine/Rendering/Material.hpp>  // TODO:
#include <LuminoEngine/Rendering/FeatureRenderer/SpriteRenderer.hpp>
#include "../../../../lumino/LuminoRuntime/src/Rendering/RenderFeature/SpriteRenderFeature.hpp"
#include "../Engine/EngineManager.hpp"  // TODO:
#include <LuminoFramework/Scene/World.hpp>  // TODO:
#include <LuminoFramework/Effect/ParticleEffectModel.hpp>  // TODO:
#include "EffectManager.hpp"

namespace ln {


//==============================================================================
// Effect

EffectEmitter* Effect::emit(EffectResource* effect, const Matrix& transform)
{
    EffectEmitter* emitter = detail::EngineDomain::engineManager()->mainWorld()->effectContext()->createEmitter(effect);
    emitter->setTransform(transform);
    return emitter;
}

EffectEmitter* Effect::emit(const Path& filePath, const Matrix& transform)
{
    auto emitter = detail::EngineDomain::effectManager()->createEmitterFromFile(filePath);
    if (!emitter) return nullptr;
    detail::EngineDomain::engineManager()->mainWorld()->effectContext()->addEmitter(emitter);
    emitter->setTransform(transform);
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

    // TODO: ポリモーフィズムで作成
    if (auto* sr = dynamic_cast<SpriteFrameEffectResource*>(model))
    {
        auto emitter = makeObject_deprecated<SpriteFrameEffectEmitter>(sr);
        m_emitters.add(emitter);
        return emitter;
    }
    else if (auto* sr = dynamic_cast<SpriteParticleModel*>(model))
    {
        auto emitter = makeObject_deprecated<detail::ParticleEffectEmitter>(sr);
        m_emitters.add(emitter);
        return emitter;
    }

    return nullptr;
}

void EffectContext::addEmitter(EffectEmitter* emitter)
{
    m_emitters.add(emitter);
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
// SpriteFrameEffectResource::Builder

//LN_BUILDER_IMPLEMENT(SpriteFrameEffectResource, EffectResource);
//SpriteFrameEffectResource::Builder::Builder() : Builder(makeRef<Details>()) {}
//SpriteFrameEffectResource::Builder::Builder(Details* d) : base::Builder(d) {}

//UIGridLayout::Builder::Builder() : Builder(makeRef<Details>()) {}
//	UIGridLayout::Builder::Builder(Details* d) : base::Builder(d) {}
//
	//type::Builder() : Builder(makeRef<type::Builder::Details>()) {} \
	//	type::Builder(type::Builder::Details* d) : base::Builder(d) {}

//UIGridLayout::Builder::Builder()
//	: Builder(makeRef<Details>())
//{
//}
//
//UIGridLayout::Builder::Builder(Details* d)
//	: UIElement::Builder(d)
//{
//}

//UIGridLayout::Builder& UIGridLayout::Builder::columnCount(int value)
//{
//	detailsAs<Details>()->columnCount = value;
//	return *this;
//}
//
//UIGridLayout::Builder& UIGridLayout::Builder::add(const UIElement::Builder& value)
//{
//	detailsAs<Details>()->children.push_back(value);
//	return *this;
//}

//Ref<UIGridLayout> UIGridLayout::Builder::build()
//{
//	return buildAs<UIGridLayout>();
//}

//Ref<Object> UIGridLayout::Builder::Details::build()
//{
//	auto ptr = makeObject_deprecated<UIGridLayout>();
//	ptr->setColumnCount(columnCount);
//	for (auto& b : children)
//		ptr->addChild(b.build());
//	return ptr;
//}

//==============================================================================
// SpriteFrameEffectResource

SpriteFrameEffectResource::SpriteFrameEffectResource()
{
}

SpriteFrameEffectResource::~SpriteFrameEffectResource()
{
}

void SpriteFrameEffectResource::init(const Size& spriteSize, SpriteSheet* spriteFrameSet, int startNumber, int lastNumber, float frameTime)
{
    EffectResource::init();
    m_spriteSize = spriteSize;
    m_spriteFrameSet = spriteFrameSet;
    m_startNumber = startNumber;
    m_lastNumber = lastNumber;
    m_frameTime = frameTime;
    m_material = makeObject_deprecated<Material>();
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

bool SpriteFrameEffectEmitter::onUpdate(float localTime, float elapsedSeconds)
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
        detail::SpriteRenderFeature2::makeRenderSizeAndSourceRectHelper(
            tex, m_data->m_spriteSize, sourceRect, &renderSize, &renderSourceRect);

       // Matrix transform = Matrix::makeTranslation(position());

        
        auto r = SpriteRenderer::get();
        r->begin(renderingContext, m_data->m_material);
        r->drawSprite(
            transform(), renderSize, anchorPoint, renderSourceRect, Color::White,
            SpriteBaseDirection::ZMinus, BillboardType::None, SpriteFlipFlags::None);
        r->end();
    }
}

} // namespace ln

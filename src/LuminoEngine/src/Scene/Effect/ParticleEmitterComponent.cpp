
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Effect/ParticleEmitterComponent.hpp>
#include "../../Effect/ParticleEffectInstance.hpp"

namespace ln {

//==============================================================================
// ParticleEmitterComponent
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ParticleEmitterComponent, VisualComponent);

//------------------------------------------------------------------------------
ParticleEmitterComponent::ParticleEmitterComponent()
	: m_model(nullptr)
{
}

//------------------------------------------------------------------------------
ParticleEmitterComponent::~ParticleEmitterComponent()
{
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::init(SpriteParticleModel* model)
{
	if (LN_REQUIRE(model != nullptr)) return;

	VisualComponent::init();
	m_model = model;
	m_model->commit();
	m_instance = m_model->createInstane();
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::onUpdate(float deltaTime)
{
	m_instance->m_worldTransform = worldObject()->worldMatrix();
	m_model->updateInstance(m_instance, deltaTime, m_instance->m_worldTransform);
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::onRender(RenderingContext* renderer)
{
	Vector3 dir = renderer->viewPoint()->viewDirection;
	m_model->render(
		renderer,
		m_instance,
        //worldObject()->worldMatrix(),
		renderer->viewPoint()->viewPosition,
		dir,
		Matrix::makeInverse(renderer->viewPoint()->viewMatrix),
		m_model->getMaterial());
}


//==============================================================================
// ParticleEmitterComponent2
//==============================================================================

ParticleEmitterComponent2::ParticleEmitterComponent2()
	: m_model(nullptr)
{
}

void ParticleEmitterComponent2::init(ParticleModel* model)
{
	if (LN_REQUIRE(model)) return;

	VisualComponent::init();
	//m_model = model;
	//m_model->commit();
	m_instance = makeObject<detail::ParticleInstance2>(model);

	setBlendMode(BlendMode::Alpha);
}

void ParticleEmitterComponent2::onUpdate(float deltaTime)
{
	m_instance->setWorldTransform(worldObject()->worldMatrix());
	m_instance->updateFrame(deltaTime);
	//m_instance->m_worldTransform = worldObject()->worldMatrix();
	//m_model->updateInstance(m_instance, deltaTime, m_instance->m_worldTransform);
}

void ParticleEmitterComponent2::onRender(RenderingContext* context)
{
	context->setBaseTransfrom(Matrix::Identity);
	m_instance->render(context);
	//Vector3 dir = renderer->viewPoint()->viewDirection;
	//m_model->render(
	//	renderer,
	//	m_instance,
	//	//worldObject()->worldMatrix(),
	//	renderer->viewPoint()->viewPosition,
	//	dir,
	//	Matrix::makeInverse(renderer->viewPoint()->viewMatrix),
	//	m_model->getMaterial());
}

} // namespace ln



#include "Internal.hpp"
#include <LuminoEngine/Visual/LightComponent.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// EnvironmentLight

LN_OBJECT_IMPLEMENT(EnvironmentLight, WorldObject) {}

EnvironmentLight::EnvironmentLight()
	: WorldObject()
	, m_component(nullptr)
{
}

EnvironmentLight::~EnvironmentLight()
{
}

void EnvironmentLight::init()
{
	WorldObject::init();
	m_component = makeObject_deprecated<EnvironmentLightComponent>();
	addComponent(m_component);
	setPosition(2.5, 10, 5);
#ifdef LN_COORD_RH
	// 右上手前から照らす ((2.5, 10, 5) から原点への向き)
	setRotation(1.06105709f, -2.67794538f, 0.0f);
#else
	setRotation(Math::degreesToRadians(50), Math::degreesToRadians(-30), 0);	// 右上手前から照らす
#endif
	// NOTE: ライトのデフォルト向きは、「とりあえずまずはオブジェクトを表示」してみたときに視認しやすい陰影が付くようにしている。
}

EnvironmentLightComponent* EnvironmentLight::environmentLightComponent() const
{
	return m_component;
}

//==============================================================================
// DirectionalLight

LN_OBJECT_IMPLEMENT(DirectionalLight, WorldObject) {}

Ref<DirectionalLight> DirectionalLight::create()
{
	return makeObject_deprecated<DirectionalLight>();
}

Ref<DirectionalLight> DirectionalLight::create(const Color& color)
{
	return makeObject_deprecated<DirectionalLight>(color);
}

DirectionalLight::DirectionalLight()
	: WorldObject()
	, m_component(nullptr)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::init()
{
	WorldObject::init();
	m_component = makeObject_deprecated<DirectionalLightComponent>();
	addComponent(m_component);
    setPosition(0, 0, 0);
	setRotation(Math::degreesToRadians(50), Math::degreesToRadians(-30), 0);	// 右上手前から照らす
    //lookAt(Vector3(1, -1, 1));


	//if (detail::EngineDomain::sceneManager()->autoAddingToActiveWorld) {
	//	World* activeWorld = detail::EngineDomain::sceneManager()->activeWorld();
	//	if (activeWorld && !activeWorld->mainDirectionalLight()) {
	//		activeWorld->setMainDirectionalLight(this);
	//	}
	//}
}

void DirectionalLight::init(const Color& color)
{
	init();
	setColor(color);
}

DirectionalLightComponent* DirectionalLight::getDirectionalLightComponent() const
{
	return m_component;
}

//==============================================================================
// PointLight

LN_OBJECT_IMPLEMENT(PointLight, WorldObject) {}

Ref<PointLight> PointLight::create()
{
	return makeObject_deprecated<PointLight>();
}

Ref<PointLight> PointLight::create(const Color& color, float range)
{
	return makeObject_deprecated<PointLight>(color, range);
}

PointLight::PointLight()
	: WorldObject()
	, m_component(nullptr)
{
}

PointLight::~PointLight()
{
}

void PointLight::init()
{
	WorldObject::init();
	m_component = makeObject_deprecated<PointLightComponent>();
	addComponent(m_component);
}

void PointLight::init(const Color& color, float range)
{
	init();
	setColor(color);
	setRange(range);
}

PointLightComponent* PointLight::getPointLightComponent() const
{
	return m_component;
}

//==============================================================================
// SpotLight

LN_OBJECT_IMPLEMENT(SpotLight, WorldObject) {}

Ref<SpotLight> SpotLight::create()
{
	return makeObject_deprecated<SpotLight>();
}

Ref<SpotLight> SpotLight::create(const Color& color, float range, float angle)
{
	return makeObject_deprecated<SpotLight>(color, range, angle);
}

SpotLight::SpotLight()
	: WorldObject()
	, m_component(nullptr)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::init()
{
	WorldObject::init();
	m_component = makeObject_deprecated<SpotLightComponent>();
	addComponent(m_component);
}

void SpotLight::init(const Color& color, float range, float angle)
{
	init();
	setColor(color);
	setRange(range);
	setAngle(angle);
}

SpotLightComponent* SpotLight::getSpotLightComponent() const
{
	return m_component;
}


} // namespace ln


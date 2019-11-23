
#include "Internal.hpp"
#include <LuminoEngine/Visual/LightComponent.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// AmbientLight

LN_OBJECT_IMPLEMENT(AmbientLight, WorldObject) {}

Ref<AmbientLight> AmbientLight::create()
{
	return makeObject<AmbientLight>();
}

Ref<AmbientLight> AmbientLight::create(const Color& color)
{
	return makeObject<AmbientLight>(color);
}

AmbientLight::AmbientLight()
	: WorldObject()
	, m_component(nullptr)
{
}

AmbientLight::~AmbientLight()
{
}

void AmbientLight::init()
{
	WorldObject::init();
	m_component = makeObject<AmbientLightComponent>();
	addComponent(m_component);
}

void AmbientLight::init(const Color& color)
{
	init();
	setColor(color);
}

AmbientLightComponent* AmbientLight::getAmbientLightComponent() const
{
	return m_component;
}

//==============================================================================
// HemisphereLight

LN_OBJECT_IMPLEMENT(HemisphereLight, WorldObject) {}

Ref<HemisphereLight> HemisphereLight::create()
{
	return makeObject<HemisphereLight>();
}

Ref<HemisphereLight> HemisphereLight::create(const Color& skyColor, const Color& groundColor)
{
	return makeObject<HemisphereLight>(skyColor, groundColor);
}

HemisphereLight::HemisphereLight()
	: WorldObject()
	, m_component(nullptr)
{
}

HemisphereLight::~HemisphereLight()
{
}

void HemisphereLight::init()
{
	WorldObject::init();
	m_component = makeObject<HemisphereLightComponent>();
	addComponent(m_component);
}

void HemisphereLight::init(const Color& skyColor, const Color& groundColor)
{
	init();
	setSkyColor(skyColor);
	setGroundColor(groundColor);
}

HemisphereLightComponent* HemisphereLight::getHemisphereLightComponent() const
{
	return m_component;
}

//==============================================================================
// DirectionalLight

LN_OBJECT_IMPLEMENT(DirectionalLight, WorldObject) {}

Ref<DirectionalLight> DirectionalLight::create()
{
	return makeObject<DirectionalLight>();
}

Ref<DirectionalLight> DirectionalLight::create(const Color& color)
{
	return makeObject<DirectionalLight>(color);
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
	m_component = makeObject<DirectionalLightComponent>();
	addComponent(m_component);
    setPosition(10, 10, -10);
    lookAt(Vector3(0, 0, 0));
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
	return makeObject<PointLight>();
}

Ref<PointLight> PointLight::create(const Color& color, float range)
{
	return makeObject<PointLight>(color, range);
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
	m_component = makeObject<PointLightComponent>();
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
	return makeObject<SpotLight>();
}

Ref<SpotLight> SpotLight::create(const Color& color, float range, float angle)
{
	return makeObject<SpotLight>(color, range, angle);
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
	m_component = makeObject<SpotLightComponent>();
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


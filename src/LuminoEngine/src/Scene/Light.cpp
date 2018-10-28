
#include "Internal.hpp"
#include <LuminoEngine/Visual/LightComponent.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Light.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// AmbientLight

Ref<AmbientLight> AmbientLight::create()
{
	return newObject<AmbientLight>();
}

Ref<AmbientLight> AmbientLight::create(const Color& color)
{
	return newObject<AmbientLight>(color);
}

AmbientLight::AmbientLight()
	: WorldObject()
	, m_component(nullptr)
{
}

AmbientLight::~AmbientLight()
{
}

void AmbientLight::initialize()
{
	WorldObject::initialize();
	m_component = newObject<AmbientLightComponent>();
	addComponent(m_component);
}

void AmbientLight::initialize(const Color& color)
{
	initialize();
	setColor(color);
}

AmbientLightComponent* AmbientLight::getAmbientLightComponent() const
{
	return m_component;
}

//==============================================================================
// HemisphereLight

Ref<HemisphereLight> HemisphereLight::create()
{
	return newObject<HemisphereLight>();
}

Ref<HemisphereLight> HemisphereLight::create(const Color& skyColor, const Color& groundColor)
{
	return newObject<HemisphereLight>(skyColor, groundColor);
}

HemisphereLight::HemisphereLight()
	: WorldObject()
	, m_component(nullptr)
{
}

HemisphereLight::~HemisphereLight()
{
}

void HemisphereLight::initialize()
{
	WorldObject::initialize();
	m_component = newObject<HemisphereLightComponent>();
	addComponent(m_component);
}

void HemisphereLight::initialize(const Color& skyColor, const Color& groundColor)
{
	initialize();
	setSkyColor(skyColor);
	setGroundColor(groundColor);
}

HemisphereLightComponent* HemisphereLight::getHemisphereLightComponent() const
{
	return m_component;
}

//==============================================================================
// DirectionalLight

Ref<DirectionalLight> DirectionalLight::create()
{
	return newObject<DirectionalLight>();
}

Ref<DirectionalLight> DirectionalLight::create(const Color& color)
{
	return newObject<DirectionalLight>(color);
}

DirectionalLight::DirectionalLight()
	: WorldObject()
	, m_component(nullptr)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::initialize()
{
	WorldObject::initialize();
	m_component = newObject<DirectionalLightComponent>();
	addComponent(m_component);
}

void DirectionalLight::initialize(const Color& color)
{
	initialize();
	setColor(color);
}

DirectionalLightComponent* DirectionalLight::getDirectionalLightComponent() const
{
	return m_component;
}

//==============================================================================
// PointLight

Ref<PointLight> PointLight::create()
{
	return newObject<PointLight>();
}

Ref<PointLight> PointLight::create(const Color& color, float range)
{
	return newObject<PointLight>(color, range);
}

PointLight::PointLight()
	: WorldObject()
	, m_component(nullptr)
{
}

PointLight::~PointLight()
{
}

void PointLight::initialize()
{
	WorldObject::initialize();
	m_component = newObject<PointLightComponent>();
	addComponent(m_component);
}

void PointLight::initialize(const Color& color, float range)
{
	initialize();
	setColor(color);
	setRange(range);
}

PointLightComponent* PointLight::getPointLightComponent() const
{
	return m_component;
}

//==============================================================================
// SpotLight

Ref<SpotLight> SpotLight::create()
{
	return newObject<SpotLight>();
}

Ref<SpotLight> SpotLight::create(const Color& color, float range, float angle)
{
	return newObject<SpotLight>(color, range, angle);
}

SpotLight::SpotLight()
	: WorldObject()
	, m_component(nullptr)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::initialize()
{
	WorldObject::initialize();
	m_component = newObject<SpotLightComponent>();
	addComponent(m_component);
}

void SpotLight::initialize(const Color& color, float range, float angle)
{
	initialize();
	setColor(color);
	setRange(range);
	setAngle(angle);
}

SpotLightComponent* SpotLight::getSpotLightComponent() const
{
	return m_component;
}


} // namespace ln


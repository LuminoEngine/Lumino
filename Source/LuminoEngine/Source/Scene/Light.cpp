
#include "../Internal.h"
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/WorldObject.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// LightComponent
//==============================================================================

//------------------------------------------------------------------------------
LightComponent::LightComponent()
	: SceneNode()
	, m_lightInfo(false)
	, m_enabled(true)
	, m_spotAngle(Math::PI * 0.25f)
	//, m_viewMatrix(Matrix::Identity)
	//, m_projMatrix(Matrix::Identity)
	//, m_viewProjMatrix(Matrix::Identity)
{
}

//------------------------------------------------------------------------------
LightComponent::~LightComponent()
{
}

//------------------------------------------------------------------------------
void LightComponent::initialize(LightType type)
{
	SceneNode::initialize();
	m_lightInfo = Ref<detail::DynamicLightInfo>::makeRef();
	m_lightInfo->m_type = type;
	m_lightInfo->m_diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_ambient.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_specular.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_shadowZFar = 1000.0f;
}

//------------------------------------------------------------------------------
//void LightComponent::updateMatrices(/*const Size& viewSize*/)
//{
//	const Matrix& worldMatrix = getOwnerObject()->transform.getWorldMatrix();
//
//	// 正面方向
//	Vector3 direction = Vector3::transformCoord(Vector3(0, 0, 1), worldMatrix);
//
//	// 注視点
//	Vector3 lookAt = worldMatrix.getPosition() + direction;
//
//	// ビュー行列
//	Vector3 up = Vector3(0, 1, 0);
//	//m_viewMatrix = Matrix::makeLookAtLH(worldMatrix.getPosition(), lookAt, up);
//
//	// プロジェクション行列の更新
//	// TODO: 視野角とnear,far
//	// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
//	//m_projMatrix = Matrix::makePerspectiveFovLH(Math::PI / 4.0f, viewSize.width / viewSize.height, 0.01f, 1000.0f);
//
//	//m_worldViewProjMatrix = getMatrix() * m_viewMatrix * m_projMatrix;
//	//m_viewProjMatrix = m_viewMatrix * m_projMatrix;
//
//	m_lightInfo->m_direction = direction;
//
//	//m_viewMatrixI = Matrix::makeInverse(m_viewMatrix);
//	//m_projMatrixI = Matrix::makeInverse(m_projMatrix);
//	//m_viewProjMatrixI = Matrix::makeInverse(m_viewProjMatrix);
//	//m_viewMatrixT = Matrix::makeTranspose(m_viewMatrix);
//	//m_projMatrixT = Matrix::makeTranspose(m_projMatrix);
//	//m_viewProjMatrixT = Matrix::makeTranspose(m_viewProjMatrix);
//	//m_viewMatrixIT = Matrix::makeTranspose(m_viewMatrixI);
//	//m_projMatrixIT = Matrix::makeTranspose(m_projMatrixI);
//	//m_viewProjMatrixIT = Matrix::makeTranspose(m_viewProjMatrixI);
//}
//
//------------------------------------------------------------------------------
void LightComponent::onPreRender(DrawList* context)
{
	if (m_enabled)
	{
		//updateMatrices();
		const Matrix& t = getOwnerObject()->transform.getWorldMatrix();
		m_lightInfo->m_position = t.getPosition();
		m_lightInfo->m_direction = t.getFront();
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// DirectionalLightComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(DirectionalLightComponent, SceneNode);

DirectionalLightComponent::DirectionalLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::initialize()
{
	SceneNode::initialize();
	m_lightInfo = Ref<detail::DynamicLightInfo>::makeRef();
	m_lightInfo->m_type = LightType::Directional;
	m_lightInfo->m_diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_intensity = 0.5f;
}

void DirectionalLightComponent::onPreRender(DrawList* context)
{
	if (m_enabled)
	{
		const Matrix& t = getOwnerObject()->transform.getWorldMatrix();
		m_lightInfo->m_position = t.getPosition();
		m_lightInfo->m_direction = t.getFront();
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// PointLightComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PointLightComponent, SceneNode);

PointLightComponent::PointLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::initialize()
{
	SceneNode::initialize();
	m_lightInfo = Ref<detail::DynamicLightInfo>::makeRef();
	m_lightInfo->m_type = LightType::Point;
	m_lightInfo->m_diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_intensity = 1.0f;
	m_lightInfo->m_range = 10.0f;
	m_lightInfo->m_attenuation = 1.0f;
}

void PointLightComponent::onPreRender(DrawList* context)
{
	if (m_enabled)
	{
		const Matrix& t = getOwnerObject()->transform.getWorldMatrix();
		m_lightInfo->m_position = t.getPosition();
		m_lightInfo->m_direction = t.getFront();
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// SpotLightComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpotLightComponent, SceneNode);

SpotLightComponent::SpotLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

SpotLightComponent::~SpotLightComponent()
{
}

void SpotLightComponent::initialize()
{
	SceneNode::initialize();
	m_lightInfo = Ref<detail::DynamicLightInfo>::makeRef();
	m_lightInfo->m_type = LightType::Spot;
	m_lightInfo->m_diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_intensity = 1.0f;
	m_lightInfo->m_range = 10.0f;
	m_lightInfo->m_attenuation = 1.0f;
	m_lightInfo->m_spotAngle = Math::PI / 3;
	m_lightInfo->m_spotPenumbra = 0.1f;
}

void SpotLightComponent::onPreRender(DrawList* context)
{
	if (m_enabled)
	{
		const Matrix& t = getOwnerObject()->transform.getWorldMatrix();
		m_lightInfo->m_position = t.getPosition();
		m_lightInfo->m_direction = t.getFront();
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// Light
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Light, WorldObject);

//------------------------------------------------------------------------------
Light::Light()
	: WorldObject()
	, m_component(nullptr)
{
}

//------------------------------------------------------------------------------
Light::~Light()
{
}

//------------------------------------------------------------------------------
void Light::initialize()
{
	WorldObject::initialize();
	m_component = newObject<LightComponent>(LightType::Directional);
	addComponent(m_component);

	// Unity Default
	Quaternion rot;
	rot.rotateX(Math::degreesToRadians(50));
	rot.rotateY(Math::degreesToRadians(-30));
	setRotation(rot);
}

//------------------------------------------------------------------------------
LightComponent* Light::GetLightComponent() const
{
	return m_component;
}

//==============================================================================
// DirectionalLight
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(DirectionalLight, WorldObject);

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
	m_component = newObject<PointLightComponent>();
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
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PointLight, WorldObject);

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
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpotLight, WorldObject);

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

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END


#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Visual/LightComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Light.hpp>

#include <LuminoEngine/GPU/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
//#include "../Rendering/RenderStage.hpp"

namespace ln {

//==============================================================================
// EnvironmentLightComponent

LN_OBJECT_IMPLEMENT(EnvironmentLightComponent, VisualComponent) {}

EnvironmentLightComponent::EnvironmentLightComponent()
	: m_color(Color::White)
	, m_ambientColor(Color(0.5, 0.5, 0.5))
	, m_skyColor(Color::Black)
	, m_groundColor(Color::Black)
	, m_intensity(0.5f)
	, m_shadowEffectiveDistance(0.0f)
	, m_shadowEffectiveDepth(0.0f)
	, m_enabled(true)
{
}

EnvironmentLightComponent::~EnvironmentLightComponent()
{
}

Result<> EnvironmentLightComponent::init() {
	return VisualComponent::init();
}

void EnvironmentLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
		if (context->world->mainLight()) {
			const Matrix& t = worldObject()->worldMatrix();
			context->addEnvironmentLightInfo(
				m_color, m_ambientColor, m_skyColor, m_groundColor, m_intensity, t.front(),
				context->world->mainLight()->environmentLightComponent() == this,
				m_shadowEffectiveDistance, m_shadowEffectiveDepth);
		}
	}
}


//==============================================================================
// DirectionalLightComponent

LN_OBJECT_IMPLEMENT(DirectionalLightComponent, VisualComponent) {}

DirectionalLightComponent::DirectionalLightComponent()
	: m_color(Color::White)
	, m_intensity(1.0f)
	, m_enabled(true)
	, m_shadowEffectiveDistance(0.0f)
	, m_shadowEffectiveDepth(0.0f)
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::init()
{
	VisualComponent::init();

#if 0
	auto shader = makeObject_deprecated<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/LightDisc.fx");
	m_material = makeObject_deprecated<Material>();
	m_material->setShader(shader);


	auto tex2 = Texture2D::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Flare1.png");
	auto shader2 = makeObject_deprecated<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/FlareSprite.fx");
	m_spriteMaterial = makeObject_deprecated<Material>();
	m_spriteMaterial->setMainTexture(tex2);
	m_spriteMaterial->setShader(shader2);
#endif
}

void DirectionalLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
		//if (context->world->mainDirectionalLight()) {
			const Matrix& t = worldObject()->worldMatrix();
			context->addDirectionalLight(
				m_color, m_intensity, t.front(),
				false,
				m_shadowEffectiveDistance, m_shadowEffectiveDepth);
		//}
	}
}

void DirectionalLightComponent::onRender(RenderingContext* context)
{

#if 0

	context->setAdditionalElementFlags(detail::RenderDrawElementTypeFlags::LightDisc);
	context->setMaterial(m_material);
	context->drawBox(3);
	//context->lastRenderDrawElement()->elementType = detail::RenderDrawElementType::LightDisc;
	
	auto pos = worldObject()->position();
	float d = Vector3::dot(pos - context->viewPoint()->viewPosition, context->viewPoint()->viewDirection);
	Vector3 f = Vector3::normalize(context->viewPoint()->viewDirection * d);
	Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
	Vector3 u = Vector3::cross(f, r);
	auto actualTransform = Matrix(
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		f.x, f.y, f.z, 0.0f,
		pos.x, pos.y, pos.z, 1.0f);
	//context->setTransfrom(actualTransform);
	context->setBlendMode(BlendMode::Add);
	context->setDepthWriteEnabled(false);
	//context->setBlendMode(BlendMode::Alpha);
	context->setBaseTransfrom(actualTransform);
	context->setMaterial(m_spriteMaterial);
	context->drawScreenRectangle();
	//context->drawPlane(1, 1);
#endif
}

//==============================================================================
// PointLightComponent

LN_OBJECT_IMPLEMENT(PointLightComponent, VisualComponent) {}

PointLightComponent::PointLightComponent()
	: m_color(Color::White)
	, m_intensity(1.0f)
	, m_range(10.0f)
	, m_attenuation(1.0f)
	, m_enabled(true)
{
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::init()
{
	VisualComponent::init();
}

void PointLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
		const Matrix& t = worldObject()->worldMatrix();
		context->addPointLight(m_color, m_intensity, t.position(), m_range, m_attenuation);
	}
}

//==============================================================================
// SpotLightComponent

LN_OBJECT_IMPLEMENT(SpotLightComponent, VisualComponent) {}

SpotLightComponent::SpotLightComponent()
	: m_color(Color::White)
	, m_intensity(1.0f)
	, m_range(10.0f)
	, m_attenuation(1.0f)
	, m_spotAngle(Math::PI / 3)
	, m_spotPenumbra(0.1f)
	, m_enabled(true)
{
}

SpotLightComponent::~SpotLightComponent()
{
}

void SpotLightComponent::init()
{
	VisualComponent::init();
}

void SpotLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
        const Matrix& t = worldObject()->worldMatrix();
		context->addSpotLight(
			m_color, m_intensity, t.position(), t.front(),
			m_range, m_attenuation, m_spotAngle, m_spotPenumbra);
	}
}

} // namespace ln

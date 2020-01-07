
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "InternalSkyBox.hpp"

namespace ln {
namespace detail {

//==============================================================================
// InternalSkyBox

InternalSkyBox::InternalSkyBox()
{
}

void InternalSkyBox::init()
{
	Object::init();
	auto shader = Shader::create(u"D:/Proj/LN/Lumino/src/LuminoEngine/src/Scene/Resource/SkyLowAltitudeOptimized.fx");

	m_material = makeObject<Material>();
	m_material->setShader(shader);
}

void InternalSkyBox::render(RenderingContext* context)
{
	const Vector3 lightDir = Vector3::normalize(0, 0.1, 1);

	const float sunDistance = 400000;
	Vector3 sunPosition = lightDir * sunDistance;// Vector3(0, 1000, sunDistance);


	{
		auto inclination = 4.5;
		auto azimuth = 0.5;// 0.250;
		auto theta = Math::PI * (inclination - 0.5);
		auto phi = 2.0 * Math::PI * (azimuth - 0.5);

		sunPosition.x = sunDistance * cos(phi);
		sunPosition.y = sunDistance * sin(phi) * sin(theta);
		sunPosition.z = sunDistance * sin(phi) * cos(theta);
	}

	m_material->setFloat(u"turbidity", 10.0f);
	m_material->setFloat(u"rayleigh", 2.0f);
	m_material->setFloat(u"mieCoefficient", 0.005f);
	m_material->setFloat(u"mieDirectionalG", 0.8f);
	m_material->setFloat(u"luminance", 1.0f);
	m_material->setVector(u"up", Vector4(0, 1, 0, 0));
	m_material->setVector(u"sunPosition", Vector4(sunPosition, 0));


	context->pushState();
	//renderingContext->setTransfrom(mm);
	context->setDepthWriteEnabled(false);
	context->setMaterial(m_material);
	//renderingContext->drawScreenRectangle();
	//context->drawMesh(m_skyProjectionPlane->meshContainers()[0]->meshResource(), 0);
	context->setCullingMode(CullMode::Front);
	context->drawBox(Box(Vector3::Zero, 100));
	context->popState();
}

} // namespace detail
} // namespace ln


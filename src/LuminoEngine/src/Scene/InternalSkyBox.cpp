
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
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
	auto shader = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Scene/Resource/SkyLowAltitudeOptimized.fx");

	m_material = makeObject<Material>();
	m_material->setShader(shader);
    m_material->shadingModel = ShadingModel::UnLighting;

    m_lightDirection = Vector3::UnitY;
}

void InternalSkyBox::setLightDirection(const Vector3& value)
{
    m_lightDirection = value;
}

void InternalSkyBox::render(RenderingContext* context, const RenderViewPoint* viewPoint)
{
	const float sunDistance = 400000;

    
    //m_material->setVector(u"_RayleighColorScale", Vector4(0.1, 0.2, 0.6, 0.0));
    m_material->setVector(u"_RayleighColorScale", Color(0.5, 0.2, 0.5).toVector4());
	//m_material->setFloat(u"turbidity", 10.0f);
    m_material->setFloat(u"turbidity", 1.0f);
    //m_material->setFloat(u"rayleigh", 2.0f);
	m_material->setFloat(u"rayleigh", 0.5f);
	m_material->setFloat(u"mieCoefficient", 0.005f);
	m_material->setFloat(u"mieDirectionalG", 0.8f);
	m_material->setFloat(u"luminance", 1.0f);
	m_material->setVector(u"up", Vector4(0, 1, 0, 0));
	m_material->setVector(u"sunPosition", Vector4(m_lightDirection * sunDistance, 0));

	context->pushState();
    //context->setRenderPhase(RenderPhaseClass::BackgroundSky);
    //context->setDepthTestEnabled(false);
	context->setDepthWriteEnabled(false);
    context->setBaseTransfrom(Matrix::makeTranslation(viewPoint->viewPosition));
	context->setMaterial(m_material);
	context->setCullingMode(CullMode::Front);
    context->drawBox(Box(Vector3::Zero, 100));// , Color::White, Matrix::makeTranslation(viewPoint->viewPosition));
	context->popState();
}

} // namespace detail
} // namespace ln



#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
#include "../SceneManager.hpp"

namespace ln {

//=============================================================================
// PlaneMeshComponent

PlaneMeshComponent::PlaneMeshComponent()
    : m_sizeX(10.0f)
    , m_sizeY(10.0f)
{
}

PlaneMeshComponent::~PlaneMeshComponent()
{
}

void PlaneMeshComponent::init()
{
    VisualComponent::init();
}

void PlaneMeshComponent::setMaterial(Material* material)
{
    m_material = material;
}

void PlaneMeshComponent::onRender(RenderingContext* context)
{
    context->setMaterial(m_material);
    context->drawPlane(m_sizeX, m_sizeY, Color::White);
}

//=============================================================================
// SphereMeshComponent

SphereMeshComponent::SphereMeshComponent()
{
}

SphereMeshComponent::~SphereMeshComponent()
{
}

void SphereMeshComponent::init()
{
    VisualComponent::init();
}

void SphereMeshComponent::setMaterial(Material* material)
{
    m_material = material;
}

void SphereMeshComponent::onRender(RenderingContext* context)
{
    context->setMaterial(m_material);
    context->drawSphere(0.5, 8, 8, Color::White);
}

//=============================================================================
// BoxMeshComponent

BoxMeshComponent::BoxMeshComponent()
{
}

BoxMeshComponent::~BoxMeshComponent()
{
}

void BoxMeshComponent::init()
{
	VisualComponent::init();
	m_material = detail::EngineDomain::sceneManager()->primitiveMeshDefaultMaterial();
	m_box = Box(1.0f);
}

void BoxMeshComponent::setMaterial(Material* material)
{
	m_material = material;
}

void BoxMeshComponent::onRender(RenderingContext* context)
{
	context->setMaterial(m_material);
	context->drawBox(m_box, Color::White);
}

} // namespace ln


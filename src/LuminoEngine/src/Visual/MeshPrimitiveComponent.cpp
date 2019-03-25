
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/MeshPrimitiveComponent.hpp>

namespace ln {

//=============================================================================
// PlaneComponent

PlaneComponent::PlaneComponent()
    : m_width(10.0f)
    , m_depth(10.0f)
{
}

PlaneComponent::~PlaneComponent()
{
}

void PlaneComponent::init()
{
    VisualComponent::init();
}

void PlaneComponent::setMaterial(Material* material)
{
    m_material = material;
}

void PlaneComponent::onRender(RenderingContext* context)
{
    context->setMaterial(m_material);
    context->drawPlane(m_width, m_depth, Color::White);
}

//=============================================================================
// SphereComponent

SphereComponent::SphereComponent()
{
}

SphereComponent::~SphereComponent()
{
}

void SphereComponent::init()
{
    VisualComponent::init();
}

void SphereComponent::setMaterial(Material* material)
{
    m_material = material;
}

void SphereComponent::onRender(RenderingContext* context)
{
    context->setMaterial(m_material);
    context->drawSphere(0.5, 8, 8, Color::White);
}

//=============================================================================
// BoxComponent

BoxComponent::BoxComponent()
{
}

BoxComponent::~BoxComponent()
{
}

void BoxComponent::init()
{
	VisualComponent::init();
}

void BoxComponent::setMaterial(Material* material)
{
	m_material = material;
}

void BoxComponent::onRender(RenderingContext* context)
{
	context->setMaterial(m_material);
	context->drawSphere(0.5, 8, 8, Color::White);
}

} // namespace ln


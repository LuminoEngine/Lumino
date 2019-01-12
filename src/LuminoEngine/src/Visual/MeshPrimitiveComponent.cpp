
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/MeshPrimitiveComponent.hpp>

namespace ln {

//=============================================================================
// SphereComponent

SphereComponent::SphereComponent()
{
}

void SphereComponent::initialize()
{
    VisualComponent::initialize();
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

} // namespace ln


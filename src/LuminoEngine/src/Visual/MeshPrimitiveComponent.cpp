
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

void SphereComponent::onRender(RenderingContext* context)
{
    context->drawSphere(1, 8, 8, Color::White);
}

} // namespace ln


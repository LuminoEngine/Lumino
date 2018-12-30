
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/StaticMeshComponent.hpp>

namespace ln {

//=============================================================================
// StaticMeshComponent

StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::initialize()
{
    VisualComponent::initialize();
}

void StaticMeshComponent::onRender(RenderingContext* context)
{
}

} // namespace ln


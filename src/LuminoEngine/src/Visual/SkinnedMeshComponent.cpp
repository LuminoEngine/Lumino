
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Visual/SkinnedMeshComponent.hpp>

namespace ln {


//=============================================================================
// SkinnedMeshComponent

SkinnedMeshComponent::SkinnedMeshComponent()
    : m_model(nullptr)
{
}

SkinnedMeshComponent::~SkinnedMeshComponent()
{
}

void SkinnedMeshComponent::initialize()
{
    VisualComponent::initialize();
}

void SkinnedMeshComponent::setModel(SkinnedMeshModel* model)
{
    m_model = model;
}

SkinnedMeshModel* SkinnedMeshComponent::model() const
{
    return m_model;
}

void SkinnedMeshComponent::onRender(RenderingContext* context)
{
}

} // namespace ln


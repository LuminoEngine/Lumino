
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
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

void SkinnedMeshComponent::init()
{
    VisualComponent::init();
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
    for (auto& meshContainer : m_model->meshContainers())
    {
        MeshResource* meshResource = meshContainer->meshResource();
        for (int iSection = 0; iSection < meshResource->sections().size(); iSection++)
        {
            context->setMaterial(m_model->materials()[meshResource->sections()[iSection].materialIndex]);
            context->drawMesh(meshResource, iSection);
        }
    }
}

} // namespace ln


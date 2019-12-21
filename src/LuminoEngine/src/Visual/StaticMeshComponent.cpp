
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Visual/StaticMeshComponent.hpp>

namespace ln {


//=============================================================================
// StaticMeshComponent

StaticMeshComponent::StaticMeshComponent()
    : m_model(nullptr)
{
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::init()
{
    VisualComponent::init();
}

void StaticMeshComponent::setModel(StaticMeshModel* model)
{
    m_model = model;
}

StaticMeshModel* StaticMeshComponent::model() const
{
    return m_model;
}

void StaticMeshComponent::onRender(RenderingContext* context)
{
    const auto& containers = m_model->meshContainers();
    for (int iContainer = 0; iContainer < containers.size(); iContainer++)
    {
        const auto& meshContainer = containers[iContainer];

        MeshResource* meshResource = meshContainer->meshResource();
        if (meshResource) {
            for (int iSection = 0; iSection < meshResource->sections().size(); iSection++) {
                context->setMaterial(m_model->materials()[meshResource->sections()[iSection].materialIndex]);
                context->drawMesh(meshResource, iSection);
            }
        }


        //Mesh* mesh = meshContainer->mesh();
        //if (mesh) {
        //    for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
        //        context->setMaterial(m_model->materials()[mesh->sections()[iSection].materialIndex]);
        //        context->drawMesh(mesh, iSection);
        //    }
        //}
    }


    for (const auto& node : m_model->meshNodes()) {
        if (node->meshContainerIndex() >= 0) {
            context->setTransfrom(m_model->nodeGlobalTransform(node->index()));


            const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];

            Mesh* mesh = meshContainer->mesh();
            if (mesh) {
                for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
                    context->setMaterial(m_model->materials()[mesh->sections()[iSection].materialIndex]);
                    context->drawMesh(mesh, iSection);
                }
            }

        }
    }
}

} // namespace ln



#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMeshComponent.hpp>

namespace ln {


//=============================================================================
// StaticMeshComponent

LN_OBJECT_IMPLEMENT(StaticMeshComponent, VisualComponent) {}

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

void StaticMeshComponent::serialize(Serializer2& ar)
{
    VisualComponent::serialize(ar);
    ar & makeNVP(u"model", m_model);
}

void StaticMeshComponent::onRender(RenderingContext* context)
{
    for (const auto& node : m_model->meshNodes()) {
        if (node->meshContainerIndex() >= 0) {

            context->setTransfrom(m_model->nodeGlobalTransform(node->index()));
            const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];

            if (meshContainer->isVisible()) {
                if (Mesh* mesh = meshContainer->mesh()) {
                    for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
                        int materialIndex = mesh->sections()[iSection].materialIndex;
                        if (materialIndex >= 0) {
                            context->setMaterial(m_model->materials()[materialIndex]);
                            context->drawMesh(mesh, iSection);
                        }
                    }
                }
            }
        }
    }
}

} // namespace ln


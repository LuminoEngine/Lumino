
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Physics/CollisionShape.hpp>
#include <LuminoEngine/Physics/RigidBody.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Scene/Mesh/StaticMeshComponent.hpp>
#include "Engine/EngineManager.hpp"

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

bool StaticMeshComponent::init()
{
    return VisualComponent::init();
}

void StaticMeshComponent::onDispose(bool explicitDisposing)
{
    deleteCollisionBody();
    VisualComponent::onDispose(explicitDisposing);
}

void StaticMeshComponent::deleteCollisionBody()
{
    if (m_body) {
        m_body->removeFromPhysicsWorld();
        m_body = nullptr;
    }
}

void StaticMeshComponent::setModel(MeshModel* model)
{
    m_model = model;
}

MeshModel* StaticMeshComponent::model() const
{
    return m_model;
}

void StaticMeshComponent::makeCollisionBody(StringRef meshContainerName)
{
    if (LN_REQUIRE(m_model)) return;

    if (auto node = m_model->findNode(u"Lumino.Collider")) {
        int index = node->meshContainerIndex();
        if (index >= 0) {
            auto meshContainer = m_model->meshContainers()[index];
            auto shape = MeshCollisionShape::create(meshContainer->mesh());
            m_body = makeObject<RigidBody>(shape);

            // TODO: onPreUpdate で UpdateContext からとりたいことろ
            detail::EngineDomain::engineManager()->mainPhysicsWorld()->addPhysicsObject(m_body);

            meshContainer->setVisible(false);
        }
    }
}

void StaticMeshComponent::serialize(Serializer2& ar)
{
    VisualComponent::serialize(ar);
    ar & makeNVP(u"model", m_model);
}

void StaticMeshComponent::onRender(RenderingContext* context)
{
    if (!m_model) return;

    for (const auto& node : m_model->meshNodes()) {
        if (node->isVisible() && node->meshContainerIndex() >= 0) {

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


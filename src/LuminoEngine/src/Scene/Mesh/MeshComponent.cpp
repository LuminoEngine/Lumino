
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Physics/CollisionShape.hpp>
#include <LuminoEngine/Physics/RigidBody.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include "Engine/EngineManager.hpp"
#include "../../Mesh/MeshModelInstance.hpp"

namespace ln {


//=============================================================================
// MeshComponent

LN_OBJECT_IMPLEMENT(MeshComponent, VisualComponent) {}

MeshComponent::MeshComponent()
    : m_model(nullptr)
{
}

MeshComponent::~MeshComponent()
{
}

bool MeshComponent::init()
{
    return VisualComponent::init();
}

void MeshComponent::onDispose(bool explicitDisposing)
{
    deleteCollisionBody();
    VisualComponent::onDispose(explicitDisposing);
}

void MeshComponent::deleteCollisionBody()
{
    if (m_body) {
        m_body->removeFromPhysicsWorld();
        m_body = nullptr;
    }
}

void MeshComponent::setModel(MeshModel* model)
{
    if (m_model != model) {
        m_modelInstance = nullptr;
        m_model = model;

        if (!m_model->isStaticMeshModel()) {
            m_modelInstance = m_model->createMeshModelInstance();
        }
    }
}

MeshModel* MeshComponent::model() const
{
    return m_model;
}

void MeshComponent::makeCollisionBody(StringRef meshContainerName)
{
    if (LN_REQUIRE(m_model)) return;

    if (auto node = m_model->findNode(u"Lumino.Collider")) {
        int index = node->meshContainerIndex();
        if (index >= 0) {
            auto meshContainer = m_model->meshContainers()[index];
            auto shape = MeshCollisionShape::create(meshContainer->meshPrimitives()[0]);
            m_body = makeObject<RigidBody>(shape);

            // TODO: onPreUpdate で UpdateContext からとりたいことろ
            detail::EngineDomain::engineManager()->mainPhysicsWorld()->addPhysicsObject(m_body);

            meshContainer->setVisible(false);
        }
    }
}

void MeshComponent::serialize(Serializer2& ar)
{
    VisualComponent::serialize(ar);
    ar & makeNVP(u"model", m_model);
}

void MeshComponent::onUpdate(float elapsedSeconds)
{

    if (m_modelInstance) {
        m_model->beginUpdate();

        // TODO: worldMatrix() を使うので、オブジェクトに追従する形にしたい。 OnLateUpdate へ
        //m_model->setWorldTransform(worldObject()->worldMatrix());

        //m_meshModel->getAnimator()->advanceTime(elapsedTime);
        if (m_model->animationController()) {
            m_model->animationController()->advanceTime(elapsedSeconds);
        }
        //m_model->animationController()->updateTargetElements();
        //static bool init = false;
        //if (!init)
        //{
        //	m_meshModel->getAnimator()->advanceTime(808.11);
        //	init = true;
        //}
        m_model->preUpdate();
        m_modelInstance->updateSkinningMatrices();



        //for (int i = 0; i < m_meshModel->bones().getCount(); i++)
        //{
        //	SkinnedMeshBone* bone = m_meshModel->bones()[i];
        //	if (bone->name() == _T("左足首"))
        //	{
        //		AttitudeTransform t;
        //		t.scale = Vector3(1, 1, 0.2);
        //		bone->setLocalTransform(t);
        //	}
        //}
    }


}

void MeshComponent::onRender(RenderingContext* context)
{
    if (!m_model) return;

    for (const auto& node : m_model->meshNodes()) {
        if (node->isVisible() && node->meshContainerIndex() >= 0) {

            context->setTransfrom(m_model->nodeGlobalTransform(node->index()));
            const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];
            if (meshContainer->isVisible()) {
                for (const auto& mesh : meshContainer->meshPrimitives()) {

                    for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
                        int materialIndex = mesh->sections()[iSection].materialIndex;
                        if (materialIndex >= 0) {
                            context->setMaterial(m_model->materials()[materialIndex]);

                            if (m_modelInstance && node->skeletonIndex >= 0) {
                                context->drawSkinnedMesh(mesh, iSection, m_modelInstance->skeletons()[node->skeletonIndex]);
                            }
                            else {
                                context->drawMesh(mesh, iSection);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MeshComponent::onRenderGizmo(RenderingContext* context)
{
    CommandList* commandList = context->getCommandList(ln::RenderPart::Gizmo, ln::detail::ProjectionKind::ViewProjection3D);

    Vector3 lines[] = {
        { -1, 1, 1 }, { 1, 1, 1 },
        { 1, 1, 1 }, { 1, 1, -1 },
    };
    commandList->drawLineList(lines, 4, Color::Red);
}

} // namespace ln


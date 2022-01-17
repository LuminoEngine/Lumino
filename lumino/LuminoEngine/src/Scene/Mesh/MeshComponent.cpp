
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Physics/CollisionShape.hpp>
#include <LuminoEngine/Physics/RigidBody.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Scene/Mesh/MeshComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
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

        //if (!m_model->isStaticMeshModel()) {
            m_modelInstance = m_model->createMeshModelInstance();
        //}
    }
}

MeshModel* MeshComponent::model() const
{
    return m_model;
}

void MeshComponent::makeCollisionBody(StringView meshContainerName)
{
    if (LN_REQUIRE(m_model)) return;

    if (auto node = m_model->findNode(_TT("Lumino.Collider"))) {
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
    ar & makeNVP(_TT("model"), m_model);
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
    CommandList* commandList = context->commandList();
    forEachVisibleMeshPrimitives(commandList, [this, commandList](MeshNode* node, MeshPrimitive* meshPrimitive) {
        for (int iSection = 0; iSection < meshPrimitive->sections().size(); iSection++) {
            int materialIndex = meshPrimitive->sections()[iSection].materialIndex;
            if (materialIndex >= 0) {
                commandList->setMaterial(m_model->materials()[materialIndex]);

                if (m_modelInstance && (node->skeletonIndex >= 0 || !m_modelInstance->morphs().isEmpty())) {

                    commandList->drawSkinnedMesh(
                        meshPrimitive, iSection, 
                        m_modelInstance->hasSkeleton() ? m_modelInstance->skeletons()[node->skeletonIndex] : nullptr,
                        m_modelInstance->hasMorph() ? m_modelInstance->morphs()[node->meshContainerIndex()] : nullptr);
                }
                else {
                    commandList->drawMesh(meshPrimitive, iSection);
                }
            }
        }
    });
}

void MeshComponent::onRenderGizmo(RenderingContext* context)
{

    CommandList* commandList = context->getCommandList(ln::RenderPart::Gizmo);

    commandList->setBaseTransfrom(worldObject()->worldMatrix());

    //const auto p = m_model->boundingBox().getCorners();
    //Vector3 lines[] = {
    //    p[0], p[1],
    //    p[1], p[2],
    //    p[2], p[3],
    //    p[3], p[0],
    //    p[4], p[5],
    //    p[5], p[6],
    //    p[6], p[7],
    //    p[7], p[4],
    //    p[0], p[4],
    //    p[1], p[5],
    //    p[2], p[6],
    //    p[3], p[7],
    //};
    //commandList->drawLineList(lines, 24, Color::Red);

    
    forEachVisibleMeshPrimitives(commandList, [this, commandList](MeshNode* node, MeshPrimitive* meshPrimitive) {
        const auto p = meshPrimitive->boundingBox().getCorners();
        Vector3 lines[] = {
            p[0], p[1],
            p[1], p[2],
            p[2], p[3],
            p[3], p[0],
            p[4], p[5],
            p[5], p[6],
            p[6], p[7],
            p[7], p[4],
            p[0], p[4],
            p[1], p[5],
            p[2], p[6],
            p[3], p[7],
        };
        commandList->drawLineList(lines, 24, Color::Red);
    });
    
}

void MeshComponent::forEachVisibleMeshPrimitives(CommandList* commandList, std::function<void(MeshNode* node, MeshPrimitive* meshPrimitive)> callback)
{
    if (!m_model) return;

    for (const auto& node : m_model->meshNodes()) {
        if (node->isVisible() && node->meshContainerIndex() >= 0) {

            commandList->setTransfrom(m_model->nodeGlobalTransform(node->index()));

            const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];
            if (meshContainer->isVisible()) {
                for (const auto& mesh : meshContainer->meshPrimitives()) {
                    callback(node, mesh);
                    //for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
                    //    int materialIndex = mesh->sections()[iSection].materialIndex;
                    //    if (materialIndex >= 0) {
                    //        context->setMaterial(m_model->materials()[materialIndex]);

                    //        if (m_modelInstance && node->skeletonIndex >= 0) {
                    //            context->drawSkinnedMesh(mesh, iSection, m_modelInstance->skeletons()[node->skeletonIndex]);
                    //        }
                    //        else {
                    //            context->drawMesh(mesh, iSection);
                    //        }
                    //    }
                    //}
                }
            }
        }
    }
}

} // namespace ln


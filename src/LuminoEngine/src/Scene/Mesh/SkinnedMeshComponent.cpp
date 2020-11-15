
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/SkinnedMeshComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "../../Mesh/MeshModelInstance.hpp"

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

bool SkinnedMeshComponent::init()
{
    return VisualComponent::init();
}

void SkinnedMeshComponent::setModel(SkinnedMeshModel* model)
{
    m_model = model;
	m_modelInstance = m_model->createMeshModelInstance();
}

SkinnedMeshModel* SkinnedMeshComponent::model() const
{
    return m_model;
}

void SkinnedMeshComponent::onUpdate(float elapsedSeconds)
{
    m_model->beginUpdate();

	// TODO: worldMatrix() を使うので、オブジェクトに追従する形にしたい。 OnLateUpdate へ
	m_model->setWorldTransform(worldObject()->worldMatrix());

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

void SkinnedMeshComponent::onRender(RenderingContext* context)
{

	m_model->postUpdate();
//#if 1
//	LN_NOTIMPLEMENTED();
//#else
//    for (auto& meshContainer : m_model->meshContainers())
//    {
//        MeshResource* meshResource = meshContainer->meshResource();
//        for (int iSection = 0; iSection < meshResource->sections().size(); iSection++)
//        {
//            context->setMaterial(m_model->materials()[meshResource->sections()[iSection].materialIndex]);
//            context->drawMesh(meshResource, iSection);
//        }
//    }
//#endif
//

	for (const auto& node : m_model->meshNodes()) {
		if (node->meshContainerIndex() >= 0 && node->isVisible()) {
			context->setTransfrom(m_model->nodeGlobalTransform(node->index()));


			const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];

			Mesh* mesh = meshContainer->mesh();
			if (mesh) {
				for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
					context->setMaterial(m_model->materials()[mesh->sections()[iSection].materialIndex]);


					if (node->skeletonIndex >= 0) {
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

} // namespace ln


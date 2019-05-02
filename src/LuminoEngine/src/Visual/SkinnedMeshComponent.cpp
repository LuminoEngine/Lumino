
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Visual/SkinnedMeshComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>

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

void SkinnedMeshComponent::onUpdate(float elapsedSeconds)
{
    m_model->beginUpdate();

	// TODO: worldMatrix() を使うので、オブジェクトに追従する形にしたい。 OnLateUpdate へ
	m_model->setWorldTransform(worldObject()->worldMatrix());

	//m_meshModel->getAnimator()->advanceTime(elapsedTime);
	m_model->animationController()->advanceTime(elapsedSeconds);
	//m_model->animationController()->updateTargetElements();
	//static bool init = false;
	//if (!init)
	//{
	//	m_meshModel->getAnimator()->advanceTime(808.11);
	//	init = true;
	//}
	m_model->preUpdate();



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



	m_model->postUpdate();
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


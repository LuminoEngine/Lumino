
#pragma once
#include "../Internal.h"
#include <Lumino/Mesh/SkinnedMeshModel.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/MeshModelObject.h>
#include "../Mesh/PmxSkinnedMesh.h"
//#include "MME/MMEShader.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SkinnedMeshComponent
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<SkinnedMeshComponent> SkinnedMeshComponent::create(const StringRef& filePath)
{
	auto ptr = RefPtr<SkinnedMeshComponent>::makeRef();
	auto mesh = SceneGraphManager::Instance->getModelManager()->createSkinnedMeshModel(filePath);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
SkinnedMeshComponent::SkinnedMeshComponent()
	: m_meshModel()
{

}

//------------------------------------------------------------------------------
SkinnedMeshComponent::~SkinnedMeshComponent()
{

}

//------------------------------------------------------------------------------
void SkinnedMeshComponent::initialize(SkinnedMeshModel* meshModel)
{
	if (LN_CHECK_ARG(meshModel != nullptr)) return;
	m_meshModel = meshModel;

	VisualComponent::initialize();

	//m_materialList->copyShared(meshModel->m_mesh->m_materials, true);


	//ownerSceneGraph->getRootNode()->addChild(this);
	setAutoRemove(true);


}

//------------------------------------------------------------------------------
SkinnedMeshModel* SkinnedMeshComponent::getSkinnedMeshModel() const
{
	return m_meshModel;
}

//------------------------------------------------------------------------------
void SkinnedMeshComponent::onUpdateFrame(float elapsedTime)
{
	// TODO: OnLateUpdate へ
	m_meshModel->setWorldTransform(getOwnerObject()->transform.getWorldMatrix());

	m_meshModel->getAnimator()->advanceTime(elapsedTime);
	//static bool init = false;
	//if (!init)
	//{
	//	m_meshModel->getAnimator()->advanceTime(808.11);
	//	init = true;
	//}
	m_meshModel->preUpdate();
	m_meshModel->postUpdate();
}

//------------------------------------------------------------------------------
//void SkinnedMesh::UpdateNodeRenderingParams(MMEShader* priorityShader)
//{
//	Shader* core = priorityShader;
//	ShaderVariable* v;
//
//	v = core->findVariable(_T("lnBoneTextureReciprocalSize"));
//	if (v) {
//		Vector4 invSize;
//		invSize.x = 1.0f / m_meshModel->getSkinningMatricesTexture()->getRealSize().width;
//		invSize.y = 1.0f / m_meshModel->getSkinningMatricesTexture()->getRealSize().height;
//		v->setVector(invSize);
//	}
//
//	v = core->findVariable(_T("lnBoneTexture"));
//	if (v) {
//		v->setTexture(m_meshModel->getSkinningMatricesTexture());
//	}
//	v = core->findVariable(_T("lnBoneLocalQuaternionTexture"));
//	if (v) {
//		v->setTexture(m_meshModel->m_skinningLocalQuaternionsTexture);
//	}
//}

//------------------------------------------------------------------------------
//void SkinnedMesh::drawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	dc->drawMesh(m_meshModel->m_meshResource, subsetIndex);
//}

//------------------------------------------------------------------------------
void SkinnedMeshComponent::onRender2(DrawList* renderer)
{
	StaticMeshModel* mesh = m_meshModel->m_mesh;


	for (int iMesh = 0; iMesh < mesh->getMeshResourceCount(); iMesh++)
	{
		MeshResource* m = mesh->getMeshResource(iMesh);
		for (int i = 0; i < m->getSubsetCount(); i++)
		{
			MeshAttribute attr;
			m->getMeshAttribute(i, &attr);
			renderer->drawMesh(m, i, mesh->getMaterial(attr.MaterialIndex));
			//renderer->drawMesh(m, i, /*GetMaterials()->GetAt(i)*/);
		}
	}

	//int subsetCount = mesh->getSubsetCount();
	//for (int i = 0; i < subsetCount; i++)
	//{
	//	renderer->drawMesh(mesh, i, mesh->getMaterial(i));
	//}
}

LN_NAMESPACE_END


//#include <Lumino/Scene/MeshModelObject.h>
//#include "MME/MMEShader.h"
//#include <Lumino/Scene/SceneGraph.h>
//#include "SceneGraphManager.h"
//
//LN_NAMESPACE_BEGIN
//
////==============================================================================
//// MeshModelObject
////==============================================================================
//
////------------------------------------------------------------------------------
//RefPtr<MeshModelObject> MeshModelObject::create(const StringRef& filePath)
//{
//	RefPtr<MeshModelObject> obj(LN_NEW MeshModelObject(), false);
//	obj->initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), filePath);	// tODO: 3Dだけ？
//	return obj;
//}
//
////------------------------------------------------------------------------------
//MeshModelObject::MeshModelObject()
//{
//}
//
////------------------------------------------------------------------------------
//MeshModelObject::~MeshModelObject()
//{
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::initialize(SceneGraph* owner, const StringRef& filePath)
//{
//	m_model.Attach(LN_NEW Model(), false);
//	m_model->create(owner->getManager()->getModelManager(), filePath);
//	VisualComponent::initialize(owner, m_model->getSubsetCount());
//
//	// マテリアルをコピーする (急ぎ足で作ったから、もっとちゃんと考えた方が良いと思う)
//	//for (int i = 0; i < m_model->getSubsetCount(); i++)
//	//{
//	//	m_visualNodeParams.GetSubsetParams(i).Material = m_model->getMaterial(i);
//	//}
//	LN_NOTIMPLEMENTED();
//
//	owner->getManager()->GetDefault3DSceneGraph()->getRootNode()->addChild(this);
//	SetAutoRemove(true);
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::onUpdateFrame(float elapsedTime)
//{
//	//m_model->getAnimator()->advanceTime((double)elapsedTime);
//	//m_model->updateBoneTransformHierarchy();
//	//m_model->updateSkinningMatrices();
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::UpdateNodeRenderingParams(MMEShader* priorityShader)
//{
//	//Shader* core = priorityShader->GetCoreShader();
//	//ShaderVariable* v;
//
//	//v = core->findVariable(_T("lnBoneTextureReciprocalSize"));
//	//if (v) {
//	//	Vector4 invSize;
//	//	invSize.X = 1.0f / m_model->getSkinningMatricesTexture()->getRealSize().Width;
//	//	invSize.Y = 1.0f / m_model->getSkinningMatricesTexture()->getRealSize().Height;
//	//	v->setVector(invSize);
//	//}
//
//	//v = core->findVariable(_T("lnBoneTexture"));
//	//if (v) {
//	//	v->setTexture(m_model->getSkinningMatricesTexture());
//	//}
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::drawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	m_model->drawSubset(subsetIndex);
//}

//LN_NAMESPACE_END

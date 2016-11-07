
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/Mesh/SkinnedMeshModel.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/MeshModelObject.h>
#include "../Modeling/PmxSkinnedMesh.h"
//#include "MME/MMEShader.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SkinnedMesh
//==============================================================================

//------------------------------------------------------------------------------
SkinnedMeshPtr SkinnedMesh::Create(const StringRef& filePath)
{
	auto ptr = SkinnedMeshPtr::MakeRef();
	auto meshResource = SceneGraphManager::Instance->GetModelManager()->CreateSkinnedMeshModel(filePath);
	auto mesh = RefPtr<SkinnedMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager(), meshResource);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
SkinnedMesh::SkinnedMesh()
	: m_meshModel()
{

}

//------------------------------------------------------------------------------
SkinnedMesh::~SkinnedMesh()
{

}

//------------------------------------------------------------------------------
void SkinnedMesh::Initialize(SceneGraph* ownerSceneGraph, SkinnedMeshModel* meshModel)
{
	LN_CHECK_ARG(meshModel != nullptr);
	m_meshModel = meshModel;

	VisualNode::Initialize(ownerSceneGraph, m_meshModel->m_materials->GetCount());

	m_materialList->CopyShared(m_meshModel->m_materials, true);


	ownerSceneGraph->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);


}

//------------------------------------------------------------------------------
SkinnedMeshModel* SkinnedMesh::GetSkinnedMeshModel() const
{
	return m_meshModel;
}

//------------------------------------------------------------------------------
void SkinnedMesh::OnUpdateFrame(float elapsedTime)
{
	m_meshModel->SetWorldTransform(m_combinedGlobalMatrix);

	m_meshModel->GetAnimator()->AdvanceTime(elapsedTime);
	//static bool init = false;
	//if (!init)
	//{
	//	m_meshModel->GetAnimator()->AdvanceTime(808.11);
	//	init = true;
	//}
	m_meshModel->PreUpdate();
	m_meshModel->PostUpdate();
}

//------------------------------------------------------------------------------
//void SkinnedMesh::UpdateNodeRenderingParams(MMEShader* priorityShader)
//{
//	Shader* core = priorityShader;
//	ShaderVariable* v;
//
//	v = core->FindVariable(_T("lnBoneTextureReciprocalSize"));
//	if (v) {
//		Vector4 invSize;
//		invSize.x = 1.0f / m_meshModel->GetSkinningMatricesTexture()->GetRealSize().width;
//		invSize.y = 1.0f / m_meshModel->GetSkinningMatricesTexture()->GetRealSize().height;
//		v->SetVector(invSize);
//	}
//
//	v = core->FindVariable(_T("lnBoneTexture"));
//	if (v) {
//		v->SetTexture(m_meshModel->GetSkinningMatricesTexture());
//	}
//	v = core->FindVariable(_T("lnBoneLocalQuaternionTexture"));
//	if (v) {
//		v->SetTexture(m_meshModel->m_skinningLocalQuaternionsTexture);
//	}
//}

//------------------------------------------------------------------------------
//void SkinnedMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	dc->DrawMesh(m_meshModel->m_meshResource, subsetIndex);
//}

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
//RefPtr<MeshModelObject> MeshModelObject::Create(const StringRef& filePath)
//{
//	RefPtr<MeshModelObject> obj(LN_NEW MeshModelObject(), false);
//	obj->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), filePath);	// tODO: 3Dだけ？
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
//void MeshModelObject::Initialize(SceneGraph* owner, const StringRef& filePath)
//{
//	m_model.Attach(LN_NEW Model(), false);
//	m_model->Create(owner->GetManager()->GetModelManager(), filePath);
//	VisualNode::Initialize(owner, m_model->GetSubsetCount());
//
//	// マテリアルをコピーする (急ぎ足で作ったから、もっとちゃんと考えた方が良いと思う)
//	//for (int i = 0; i < m_model->GetSubsetCount(); i++)
//	//{
//	//	m_visualNodeParams.GetSubsetParams(i).Material = m_model->GetMaterial(i);
//	//}
//	LN_NOTIMPLEMENTED();
//
//	owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
//	SetAutoRemove(true);
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::OnUpdateFrame(float elapsedTime)
//{
//	//m_model->GetAnimator()->AdvanceTime((double)elapsedTime);
//	//m_model->UpdateBoneTransformHierarchy();
//	//m_model->UpdateSkinningMatrices();
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::UpdateNodeRenderingParams(MMEShader* priorityShader)
//{
//	//Shader* core = priorityShader->GetCoreShader();
//	//ShaderVariable* v;
//
//	//v = core->FindVariable(_T("lnBoneTextureReciprocalSize"));
//	//if (v) {
//	//	Vector4 invSize;
//	//	invSize.X = 1.0f / m_model->GetSkinningMatricesTexture()->GetRealSize().Width;
//	//	invSize.Y = 1.0f / m_model->GetSkinningMatricesTexture()->GetRealSize().Height;
//	//	v->SetVector(invSize);
//	//}
//
//	//v = core->FindVariable(_T("lnBoneTexture"));
//	//if (v) {
//	//	v->SetTexture(m_model->GetSkinningMatricesTexture());
//	//}
//}
//
////------------------------------------------------------------------------------
//void MeshModelObject::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	m_model->DrawSubset(subsetIndex);
//}

//LN_NAMESPACE_END

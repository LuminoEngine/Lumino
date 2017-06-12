
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
RefPtr<SkinnedMeshComponent> SkinnedMeshComponent::Create(const StringRef& filePath)
{
	auto ptr = RefPtr<SkinnedMeshComponent>::MakeRef();
	auto mesh = SceneGraphManager::Instance->GetModelManager()->CreateSkinnedMeshModel(filePath);
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

	//m_materialList->CopyShared(meshModel->m_mesh->m_materials, true);


	//ownerSceneGraph->GetRootNode()->AddChild(this);
	SetAutoRemove(true);


}

//------------------------------------------------------------------------------
SkinnedMeshModel* SkinnedMeshComponent::GetSkinnedMeshModel() const
{
	return m_meshModel;
}

//------------------------------------------------------------------------------
void SkinnedMeshComponent::OnUpdateFrame(float elapsedTime)
{
	// TODO: OnLateUpdate へ
	m_meshModel->SetWorldTransform(GetOwnerObject()->transform.GetWorldMatrix());

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

//------------------------------------------------------------------------------
void SkinnedMeshComponent::OnRender2(DrawList* renderer)
{
	StaticMeshModel* mesh = m_meshModel->m_mesh;


	for (int iMesh = 0; iMesh < mesh->GetMeshResourceCount(); iMesh++)
	{
		MeshResource* m = mesh->GetMeshResource(iMesh);
		for (int i = 0; i < m->GetSubsetCount(); i++)
		{
			MeshAttribute attr;
			m->GetMeshAttribute(i, &attr);
			renderer->DrawMesh(m, i, mesh->GetMaterial(attr.MaterialIndex));
			//renderer->DrawMesh(m, i, /*GetMaterials()->GetAt(i)*/);
		}
	}

	//int subsetCount = mesh->GetSubsetCount();
	//for (int i = 0; i < subsetCount; i++)
	//{
	//	renderer->DrawMesh(mesh, i, mesh->GetMaterial(i));
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
//RefPtr<MeshModelObject> MeshModelObject::Create(const StringRef& filePath)
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
//	m_model->Create(owner->GetManager()->GetModelManager(), filePath);
//	VisualComponent::initialize(owner, m_model->GetSubsetCount());
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

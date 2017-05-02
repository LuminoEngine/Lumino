
#pragma once
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
class SkinnedMeshModel;
class SkinnedMeshComponent;

/**
	@brief
*/
class SkinnedMeshComponent
	: public VisualComponent
{
public:
	static RefPtr<SkinnedMeshComponent> Create(const StringRef& filePath);

public:
	SkinnedMeshModel* GetSkinnedMeshModel() const;

LN_INTERNAL_ACCESS:
	SkinnedMeshComponent();
	virtual ~SkinnedMeshComponent();
	void Initialize(SceneGraph* ownerSceneGraph, SkinnedMeshModel* meshModel);
	virtual void OnUpdateFrame(float elapsedTime) override;
	virtual void OnRender2(DrawList* renderer) override;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual detail::SceneNodeDefaultShaderClass GetShaderClass() override { return detail::SceneNodeDefaultShaderClass_SkinnedMesh; }

private:
	RefPtr<SkinnedMeshModel>	m_meshModel;
};

LN_NAMESPACE_END

//#include "../../../src/Modeling/Model.h"	// TODO
//#include "VisualNode.h"
//
//LN_NAMESPACE_BEGIN
//
///**
//	@brief		
//*/
//class MeshModelObject
//	: public VisualComponent
//{
//public:
//	static RefPtr<MeshModelObject> Create(const StringRef& filePath);
//
//public:
//
//public:
//
//
//	/// このモデルに対するアニメーションを行うクラスの取得
//	Animator* GetAnimator() { return m_model->GetAnimator(); }
//
//protected:
//	MeshModelObject();
//	virtual ~MeshModelObject();
//	void Initialize(SceneGraph* owner, const StringRef& filePath);
//
//	virtual void OnUpdateFrame(float elapsedTime) override;
//	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) override;
//	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;
//
//private:
//	RefPtr<Model>	m_model;
//};

//LN_NAMESPACE_END

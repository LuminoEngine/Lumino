
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
	static Ref<SkinnedMeshComponent> create(const StringRef& filePath);

public:
	SkinnedMeshModel* getSkinnedMeshModel() const;

LN_INTERNAL_ACCESS:
	SkinnedMeshComponent();
	virtual ~SkinnedMeshComponent();
	void initialize(SkinnedMeshModel* meshModel);
	void initialize(const StringRef& filePath);
	virtual void onUpdateFrame(float elapsedTime) override;
	virtual void onRender2(RenderingContext* renderer) override;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual detail::SceneNodeDefaultShaderClass getShaderClass() override { return detail::SceneNodeDefaultShaderClass_SkinnedMesh; }

private:
	Ref<SkinnedMeshModel>	m_meshModel;
};

/**
	@brief	
*/
class SkinnedMesh
	: public VisualObject
{
public:
	static Ref<SkinnedMesh> create(const StringRef& filePath);

public:
	SkinnedMeshComponent* skinnedMeshComponent() const { return m_component; }

protected:
	virtual VisualComponent* getMainVisualComponent() const override;

LN_CONSTRUCT_ACCESS:
	SkinnedMesh();
	virtual ~SkinnedMesh();
	void initialize(const StringRef& filePath);

private:
	Ref<SkinnedMeshComponent> m_component;
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
//	static Ref<MeshModelObject> create(const StringRef& filePath);
//
//public:
//
//public:
//
//
//	/// このモデルに対するアニメーションを行うクラスの取得
//	Animator* getAnimator() { return m_model->getAnimator(); }
//
//protected:
//	MeshModelObject();
//	virtual ~MeshModelObject();
//	void initialize(SceneGraph* owner, const StringRef& filePath);
//
//	virtual void onUpdateFrame(float elapsedTime) override;
//	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) override;
//	virtual void drawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;
//
//private:
//	Ref<Model>	m_model;
//};

//LN_NAMESPACE_END

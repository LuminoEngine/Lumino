
#pragma once
#include "../../../src/Modeling/Model.h"	// TODO
#include "VisualNode.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class MeshModelObject
	: public VisualNode
{
public:
	static RefPtr<MeshModelObject> Create(const StringRef& filePath);

public:

public:


	/// このモデルに対するアニメーションを行うクラスの取得
	Animator* GetAnimator() { return m_model->GetAnimator(); }

protected:
	MeshModelObject();
	virtual ~MeshModelObject();
	void Initialize(SceneGraphManager* manager, const StringRef& filePath);

	virtual void UpdateFrame(float elapsedTime) override;
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) override;
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	RefPtr<Model>	m_model;
};

LN_NAMESPACE_END

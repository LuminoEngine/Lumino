
#pragma once
#include "../../../src/Modeling/Model.h"	// TODO
#include "VisualNode.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class MeshNode
	: public VisualNode
{
public:
	MeshNode();
	virtual ~MeshNode();

public:

	/// 作成
	void Create(const TCHAR* filePath, SceneGraphManager* manager = NULL);

	/// このモデルに対するアニメーションを行うクラスの取得
	Animation::Animator* GetAnimator() { return m_model->GetAnimator(); }

protected:
	virtual void UpdateFrame(float elapsedTime) override;
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) override;
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	RefPtr<Model>	m_model;
};

LN_NAMESPACE_END

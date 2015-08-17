
#pragma once

#include "../Modeling/Model.h"
#include "VisualNode.h"

namespace Lumino
{
namespace Scene
{

/// Model
class Model
	: public VisualNode
{
public:
	Model();
	virtual ~Model();

public:

	/// 作成
	void Create(const TCHAR* filePath, SceneGraphManager* manager = NULL);

	/// このモデルに対するアニメーションを行うクラスの取得
	Animation::Animator* GetAnimator() { return m_model->GetAnimator(); }

protected:
	virtual void UpdateFrame(float elapsedTime);
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader);
	virtual void DrawSubset(RenderingParams& params, int subsetIndex);

private:
	RefPtr<Modeling::Model>	m_model;
};

} // namespace Scene
} // namespace Lumino

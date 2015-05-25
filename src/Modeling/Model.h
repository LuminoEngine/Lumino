
#pragma once
#include <Lumino/IO/PathName.h>
#include "../Animation/Animator.h"
#include "ModelCore.h"
#include "ModelBone.h"

namespace Lumino
{
namespace Modeling
{

/// Model
class Model
	: public RefObject
	, public Animation::IAnimationTargetElement
{
public:
	Model();
	virtual ~Model();

public:

	/// 作成
	void Create(ModelManager* manager, const PathName& filePath);

	/// このモデルに対するアニメーションを行うクラスの取得
	Animation::Animator* GetAnimator() { return m_animator; }

	/// ボーン行列を、ルートボーンから階層的に更新する
	/// (アニメーション適用後に呼び出す)
	void UpdateBoneTransformHierarchy();

	/// スキニングに使用する最終ボーン行列の作成
	void UpdateSkinningMatrices();

	/// スキニング行列配列の取得 (要素数は要素数はボーン数。これをそのままスキニングテクスチャに書き込める)
	Matrix* GetSkinningMatrices() { return m_skinningMatrices; }

	/// スキニング行列配列を書き込んだテクスチャの取得
	Graphics::Texture* GetSkinningMatricesTexture() { return m_skinningMatricesTexture; }

	/// サブセット数の取得
	int GetSubsetCount() const;

	/// マテリアル取得
	const Graphics::Material& GetMaterial(int subsetIndex) const;

	/// サブセット描画
	void DrawSubset(int subsetIndex);

protected:
	virtual int GetAnimationTargetAttributeCount() const { return m_boneList.GetCount(); }
	virtual Animation::IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) { return m_boneList[index]; }

private:
	ModelManager*				m_manager;
	RefPtr<ModelCore>			m_modelCore;
	RefPtr<Animation::Animator>	m_animator;		// TODO: ボーンアニメと表情アニメは分けるべきかも？
	ModelBoneList				m_boneList;					///< 全ボーンリスト
	ModelBoneList				m_rootBoneList;				///< ルートボーンリスト (親を持たないボーンリスト)
	Matrix*						m_skinningMatrices;			///< キニングに使用する最終ボーン行列 (要素数はボーン数)
	RefPtr<Graphics::Texture>	m_skinningMatricesTexture;	///< Texture fetch による GPU スキニング用のテクスチャ
	
};

} // namespace Modeling
} // namespace Lumino

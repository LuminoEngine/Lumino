
#pragma once
#include "../../src/Animation/AnimationState.h"		//  TODO
#include "../../src/Animation/Animator.h"		//  TODO
#include "../Mesh.h"

LN_NAMESPACE_BEGIN
class Animator;
class PmxSkinnedMeshResource;	// TODO: 抽象化したい
class PmxBoneResource;			// TODO: 抽象化したい
class SkinnedMeshModel;
class SkinnedMeshBone;
using SkinnedMeshModelPtr = RefPtr<SkinnedMeshModel>;
using SkinnedMeshBonePtr = RefPtr<SkinnedMeshBone>;

/**
	@brief
*/
class SkinnedMeshModel
	: public Object
	, public IAnimationTargetElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	// TODO: Unity では Mesh からは切り離された独立したコンポーネントである。そうしたほうがいいかな？
	Animator* GetAnimator() const { return m_animator; }

protected:
	// IAnimationTargetElement interface
	virtual int GetAnimationTargetAttributeCount() const override;
	virtual IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) override;

LN_INTERNAL_ACCESS:
	SkinnedMeshModel();
	virtual ~SkinnedMeshModel();
	void Initialize(GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh);


	// ボーン行列を、ルートボーンから階層的に更新する
	// (アニメーション適用後に呼び出す)
	void UpdateBoneTransformHierarchy();

	// スキニングに使用する最終ボーン行列の作成
	void UpdateSkinningMatrices();

	// スキニング行列配列の取得 (要素数は要素数はボーン数。これをそのままスキニングテクスチャに書き込める)
	//Matrix* GetSkinningMatrices() { return m_skinningMatrices; }

	// スキニング行列配列を書き込んだテクスチャの取得
	Texture* GetSkinningMatricesTexture() { return m_skinningMatricesTexture; }

	// サブセット数の取得
	//int GetSubsetCount() const;

	// マテリアル取得
	//const Material& GetMaterial(int subsetIndex) const;

	// サブセット描画
	//void DrawSubset(int subsetIndex);

LN_INTERNAL_ACCESS:	// TODO:
	RefPtr<PmxSkinnedMeshResource>	m_meshResource;
	RefPtr<MaterialList>			m_materials;
	Array<SkinnedMeshBonePtr>		m_allBoneList;				// 全ボーンリスト
	Array<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
	Array<Matrix>					m_skinningMatrices;			// スキニングに使用する最終ボーン行列 (要素数はボーン数)
	RefPtr<Texture2D>				m_skinningMatricesTexture;	// Texture fetch による GPU スキニング用のテクスチャ
	RefPtr<Animator>				m_animator;
};


/**
	@brief
*/
class SkinnedMeshBone
	: public Object
	, public IAnimationTargetAttribute
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

LN_INTERNAL_ACCESS:
	SkinnedMeshBone();
	virtual ~SkinnedMeshBone();
	void Initialize(PmxBoneResource* boneResource);

	// PmxBoneResource の取得
	PmxBoneResource* GetCore() const;

	// 子ボーンの追加
	void AddChildBone(SkinnedMeshBone* bone) { m_children.Add(bone); }

	// ボーン行列を階層的に更新する
	void UpdateTransformHierarchy(const Matrix& parentMatrix);

	//  結合済み行列 (モデル内のグローバル行列) の取得
	const Matrix& GetCombinedMatrix() const { return m_combinedMatrix; }

	// ローカル行列を初期値に戻す
	void ResetLocalTransform() { m_localTransform = SQTTransform::Identity; }

protected:
	// IAnimationTargetAttribute interface
	virtual const String& GetAnimationTargetName() const override;
	virtual void SetAnimationTargetValue(ValueType type, const void* value) override;

private:
	RefPtr<PmxBoneResource>	m_core;				// 共有データクラス
	Array<SkinnedMeshBone*>	m_children;			// 子ボーンリスト
	SQTTransform			m_localTransform;	// モーションを書き込むのはここ
	Matrix					m_combinedMatrix;	// 結合済み行列 (モデル内のグローバル行列)
};

LN_NAMESPACE_END

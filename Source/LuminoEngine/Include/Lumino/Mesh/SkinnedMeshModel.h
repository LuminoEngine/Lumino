
#pragma once
#include "../../../Source/Animation/AnimationState.h"		//  TODO
#include "../../../Source/Animation/Animator.h"		//  TODO
#include "Mesh.h"

LN_NAMESPACE_BEGIN
class Animator;
class PmxSkinnedMeshResource;	// TODO: 抽象化したい
class PmxBoneResource;			// TODO: 抽象化したい
class PmxIKResource;
class SkinnedMeshModel;
class SkinnedMeshBone;
using SkinnedMeshModelPtr = RefPtr<SkinnedMeshModel>;
using SkinnedMeshBonePtr = RefPtr<SkinnedMeshBone>;

class RigidBody;	// TODO: MMD でのみ必要
class DofSpringJoint;		// TODO: MMD でのみ必要
namespace detail { class PhysicsWorld; }	// TODO: MMD でのみ必要
namespace detail { class MmdSkinnedMeshRigidBody; }
namespace detail { class MmdSkinnedMeshJoint; }
class PmxRigidBodyResource;
class PmxJointResource;


/**
	@brief
*/
class SkinnedMeshModel
	: public Object
	, public detail::IAnimationTargetElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	// TODO: Unity では Mesh からは切り離された独立したコンポーネントである。そうしたほうがいいかな？
	Animator* GetAnimator() const { return m_animator; }

protected:
	// IAnimationTargetElement interface
	virtual int GetAnimationTargetAttributeCount() const override;
	virtual detail::IAnimationTargetAttribute* GetAnimationTargetAttribute(int index) override;

LN_INTERNAL_ACCESS:
	SkinnedMeshModel();
	virtual ~SkinnedMeshModel();
	void Initialize(detail::GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh);

	void SetWorldTransform(const Matrix& matrix) { m_worldTransform = matrix; m_worldTransformInverse = Matrix::MakeInverse(m_worldTransform); }
	const Matrix& GetWorldTransform() const { return m_worldTransform; }
	const Matrix& GetWorldTransformInverse() const { return m_worldTransformInverse; }


	void PreUpdate();

	void PostUpdate();



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

private:
	void UpdateIK();
	void UpdateBestow();


LN_INTERNAL_ACCESS:	// TODO:
	// TODO: ↓このあたりは StaticMeshModel にして、Renderer::DrawMesh に渡せるようにしたい。LOD の選択はそちらで。
	//RefPtr<PmxSkinnedMeshResource>	m_meshResource;
	//RefPtr<MaterialList>			m_materials;
	RefPtr<StaticMeshModel>			m_mesh;
	RefPtr<PmxSkinnedMeshResource>	m_meshResource;

	List<SkinnedMeshBonePtr>		m_allBoneList;				// 全ボーンリスト
	List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
	List<Matrix>					m_skinningMatrices;			// スキニングに使用する最終ボーン行列 (要素数はボーン数)
	List<Quaternion>				m_skinningLocalQuaternions;
	RefPtr<Texture2D>				m_skinningMatricesTexture;	// Texture fetch による GPU スキニング用のテクスチャ
	RefPtr<Texture2D>				m_skinningLocalQuaternionsTexture;	// Texture fetch による GPU スキニング用のテクスチャ
	RefPtr<Animator>				m_animator;
	List<SkinnedMeshBone*>			m_ikBoneList;

	// TODO: これは物理演算機能を持つサブクラスを作ったほうがいい気がする
	RefPtr<detail::PhysicsWorld>	m_physicsWorld;
	List<RefPtr<detail::MmdSkinnedMeshRigidBody>>	m_rigidBodyList;
	List<RefPtr<detail::MmdSkinnedMeshJoint>>		m_jointList;

	Matrix		m_worldTransform;
	Matrix		m_worldTransformInverse;
};


/**
	@brief
*/
class SkinnedMeshBone
	: public Object
	, public detail::IAnimationTargetAttribute
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	SkinnedMeshBone* GetParent() const { return m_parent; }

LN_INTERNAL_ACCESS:
	SkinnedMeshBone();
	virtual ~SkinnedMeshBone();
	void Initialize(PmxBoneResource* boneResource);
	void PostInitialize(SkinnedMeshModel* owner, int depth);

	// PmxBoneResource の取得
	PmxBoneResource* GetCore() const;

	// 子ボーンの追加
	void AddChildBone(SkinnedMeshBone* bone);

	// ボーン行列を階層的に更新する
	void UpdateGlobalTransform(bool hierarchical);

	//  結合済み行列 (モデル内のグローバル行列) の取得
	const Matrix& GetCombinedMatrix() const { return m_combinedMatrix; }

	// ローカル行列を初期値に戻す
	void ResetLocalTransform() { m_localTransform = SQTTransform::Identity; }

	SQTTransform* GetLocalTransformPtr() { return &m_localTransform; }

protected:
	// IAnimationTargetAttribute interface
	virtual const String& GetAnimationTargetName() const override;
	virtual void SetAnimationTargetValue(ValueType type, const void* value) override;

LN_INTERNAL_ACCESS:	// TODO
	RefPtr<PmxBoneResource>	m_core;				// 共有データクラス
	SkinnedMeshBone*		m_parent;
	List<SkinnedMeshBone*>	m_children;			// 子ボーンリスト
	SQTTransform			m_localTransform;	// モーションを書き込むのはここ
	Matrix					m_combinedMatrix;	// 結合済み行列 ()
	int						m_depth;			// 0 から
	PmxIKResource*			m_ikInfo;

	friend class SkinnedMeshModel;
};

namespace detail
{

class MmdSkinnedMeshRigidBody
	: public RefObject
{
LN_INTERNAL_ACCESS:
	MmdSkinnedMeshRigidBody();
	virtual ~MmdSkinnedMeshRigidBody();
	void Initialize(SkinnedMeshModel* ownerModel, PmxRigidBodyResource* rigidBodyResource, float scale);

	RigidBody* GetRigidBody() const;
	void UpdateBeforePhysics();
	void UpdateAfterPhysics();

private:
	SkinnedMeshModel*		m_ownerModel;
	PmxRigidBodyResource*	m_resource;
	SkinnedMeshBone*		m_bone;
	RefPtr<RigidBody>		m_rigidBody;
	Matrix					m_boneLocalPosition;
	Matrix					m_boneOffset;
	Matrix					m_offsetBodyToBone;
};

class MmdSkinnedMeshJoint
	: public RefObject
{
LN_INTERNAL_ACCESS:
	MmdSkinnedMeshJoint();
	virtual ~MmdSkinnedMeshJoint();
	void Initialize(SkinnedMeshModel* ownerModel, PmxJointResource* jointResource);

private:
	RefPtr<DofSpringJoint>	m_joint;
};

}

LN_NAMESPACE_END

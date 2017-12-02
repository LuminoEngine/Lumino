
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
using SkinnedMeshModelPtr = Ref<SkinnedMeshModel>;
using SkinnedMeshBonePtr = Ref<SkinnedMeshBone>;

class RigidBody;	// TODO: MMD でのみ必要
class DofSpringJoint;		// TODO: MMD でのみ必要
class PhysicsWorld;	// TODO: MMD でのみ必要
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
	LN_OBJECT;
public:

	// TODO: Unity では Mesh からは切り離された独立したコンポーネントである。そうしたほうがいいかな？
	Animator* getAnimator() const { return m_animator; }

protected:
	// IAnimationTargetElement interface
	virtual int getAnimationTargetAttributeCount() const override;
	virtual detail::IAnimationTargetAttribute* getAnimationTargetAttribute(int index) override;

LN_INTERNAL_ACCESS:
	SkinnedMeshModel();
	virtual ~SkinnedMeshModel();
	void initialize(detail::GraphicsManager* manager, PmxSkinnedMeshResource* sharingMesh);

	void setWorldTransform(const Matrix& matrix) { m_worldTransform = matrix; m_worldTransformInverse = Matrix::makeInverse(m_worldTransform); }
	const Matrix& getWorldTransform() const { return m_worldTransform; }
	const Matrix& getWorldTransformInverse() const { return m_worldTransformInverse; }


	void preUpdate();

	void postUpdate();



	// ボーン行列を、ルートボーンから階層的に更新する
	// (アニメーション適用後に呼び出す)
	void updateBoneTransformHierarchy();

	// スキニングに使用する最終ボーン行列の作成
	void updateSkinningMatrices();

	// スキニング行列配列の取得 (要素数は要素数はボーン数。これをそのままスキニングテクスチャに書き込める)
	//Matrix* GetSkinningMatrices() { return m_skinningMatrices; }

	// スキニング行列配列を書き込んだテクスチャの取得
	Texture* getSkinningMatricesTexture() { return m_skinningMatricesTexture; }

	// サブセット数の取得
	//int getSubsetCount() const;

	// マテリアル取得
	//const CommonMaterial& getMaterial(int subsetIndex) const;

	// サブセット描画
	//void drawSubset(int subsetIndex);

private:
	void updateIK();
	void updateBestow();


LN_INTERNAL_ACCESS:	// TODO:
	// TODO: ↓このあたりは StaticMeshModel にして、Renderer::drawMesh に渡せるようにしたい。LOD の選択はそちらで。
	//Ref<PmxSkinnedMeshResource>	m_meshResource;
	//Ref<MaterialList>			m_materials;
	Ref<StaticMeshModel>			m_mesh;
	Ref<PmxSkinnedMeshResource>	m_meshResource;

	List<SkinnedMeshBonePtr>		m_allBoneList;				// 全ボーンリスト
	List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
	List<Matrix>					m_skinningMatrices;			// スキニングに使用する最終ボーン行列 (要素数はボーン数)
	List<Quaternion>				m_skinningLocalQuaternions;
	Ref<Texture2D>				m_skinningMatricesTexture;	// Texture fetch による GPU スキニング用のテクスチャ
	Ref<Texture2D>				m_skinningLocalQuaternionsTexture;	// Texture fetch による GPU スキニング用のテクスチャ
	Ref<Animator>				m_animator;
	List<SkinnedMeshBone*>			m_ikBoneList;

	// TODO: これは物理演算機能を持つサブクラスを作ったほうがいい気がする
	Ref<PhysicsWorld>	m_physicsWorld;
	List<Ref<detail::MmdSkinnedMeshRigidBody>>	m_rigidBodyList;
	List<Ref<detail::MmdSkinnedMeshJoint>>		m_jointList;

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
	LN_OBJECT;
public:
	SkinnedMeshBone* getParent() const { return m_parent; }

LN_INTERNAL_ACCESS:
	SkinnedMeshBone();
	virtual ~SkinnedMeshBone();
	void initialize(PmxBoneResource* boneResource);
	void postInitialize(SkinnedMeshModel* owner, int depth);

	// PmxBoneResource の取得
	PmxBoneResource* getCore() const;

	// 子ボーンの追加
	void addChildBone(SkinnedMeshBone* bone);

	// ボーン行列を階層的に更新する
	void updateGlobalTransform(bool hierarchical);

	//  結合済み行列 (モデル内のグローバル行列) の取得
	const Matrix& getCombinedMatrix() const { return m_combinedMatrix; }

	// ローカル行列を初期値に戻す
	void resetLocalTransform() { m_localTransform = AttitudeTransform::Identity; }

	AttitudeTransform* getLocalTransformPtr() { return &m_localTransform; }

protected:
	// IAnimationTargetAttribute interface
	virtual const String& getAnimationTargetName() const override;
	virtual void setAnimationTargetValue(ValueType type, const void* value) override;

LN_INTERNAL_ACCESS:	// TODO
	Ref<PmxBoneResource>	m_core;				// 共有データクラス
	SkinnedMeshBone*		m_parent;
	List<SkinnedMeshBone*>	m_children;			// 子ボーンリスト
	AttitudeTransform		m_localTransform;	// モーションを書き込むのはここ
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
	void initialize(SkinnedMeshModel* ownerModel, PmxRigidBodyResource* rigidBodyResource, float scale);

	RigidBody* getRigidBody() const;
	void updateBeforePhysics();
	void updateAfterPhysics();

private:
	SkinnedMeshModel*		m_ownerModel;
	PmxRigidBodyResource*	m_resource;
	SkinnedMeshBone*		m_bone;
	Ref<RigidBody>		m_rigidBody;
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
	void initialize(SkinnedMeshModel* ownerModel, PmxJointResource* jointResource);

private:
	Ref<DofSpringJoint>	m_joint;
};

}

LN_NAMESPACE_END

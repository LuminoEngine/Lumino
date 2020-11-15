
#pragma once
#include "../Animation/AnimationMixer.hpp"
#include "Common.hpp"
#include "StaticMeshModel.hpp"

namespace ln {
class SkinnedMeshModel;
class MeshArmature;
class Texture2D;
class AnimationController;

namespace detail {
class MeshModelInstance;
}
#if 0
namespace detail {
    class PmxBoneResource; 
    class PmxIKResource;
}

class SkinnedMeshBoneResource
	: public Object
{
};


class SkinnedMeshBone
	: public Object
{
public:
	const String& name() const;

    void addChildBone(SkinnedMeshBone* bone);
    void postInitialize(SkinnedMeshModel* owner, int depth);


	// ボーン行列を階層的に更新する
	void updateGlobalTransform(bool hierarchical);

	//  結合済み行列 (モデル内のグローバル行列) の取得
	const Matrix& getCombinedMatrix() const { return m_combinedMatrix; }

	// ローカル行列を初期値に戻す
	void resetLocalTransform() { m_localTransform = AttitudeTransform::Identity; }

	AttitudeTransform* localTransformPtr() { return &m_localTransform; }



    // TODO: internal
    Ref<detail::PmxBoneResource> m_data;
    int m_boneIndex = 0;
    int m_depth = 0;






    SkinnedMeshBone*		m_parent = nullptr;
    List<SkinnedMeshBone*>	m_children;			// 子ボーンリスト
    Ref<detail::PmxIKResource>			m_ikInfo;

	AttitudeTransform m_localTransform;	// モーションを書き込むのはここ
	Matrix m_combinedMatrix;	// 結合済み行列 ()
};
#endif


// MeshNode を参照するためのデータ構造。
// Node と Bone は似ているが異なるものなので注意。頂点の BLEND_INDICES から参照されるのはこのインスタンス。
// ひとつの Node を複数の Bone が参照する。
// 正直、Bone という名前はやめた方がいい気がする。
// SkinnedMesh でのみ使用する。
class MeshBone
	: public Object
{
public:
	//const String& name() const;

	MeshNode* node() const;
	int nodeIndex() const { return m_node; }

	//const AttitudeTransform& localTransform() const;

	//const Matrix& globalMatrix() const;

public:	// TODO
	MeshArmature* m_skeleton;
	int m_node = -1;

	// ボーンの初期姿勢を打ち消して、原点に戻す行列。
	// モデル空間内の絶対座標系となっている。(親ボーンからの相対座標系ではない点に注意)
	Matrix m_inverseInitialMatrix;

	friend class MeshArmature;
};


class MeshBoneIKChain
	: public Object
{
public:

	int LinkBoneIndex;        // IK構成ボーン番号
	bool IsRotateLimit;        // 回転制限をするか
	Vector3 MinLimit;            // 下限
	Vector3 MaxLimit;            // 上限
};

class MeshBoneIK
	: public Object
{
public:
	int            IKBoneIndex;            // IKボーン (PMX では、この IK 情報を持つボーンを指す)
	int            IKTargetBoneIndex;        // IKターゲットボーン
	int            LoopCount;                // 演算回数
	float        IKRotateLimit;            // IKループ計算時の1回あたりの制限角度 -> ラジアン角 | PMDのIK値とは4倍異なるので注意

	List<Ref<MeshBoneIKChain>> IKLinks;            // IK影響ボーンと制限のリスト
};

// Bone をまとめるデータ構造。
// BoneTexture を生成する単位。
// ひとつの Model に対して複数インスタンスを持つことがあるが、MeshContainer がどの Skeleton を使って描画するのかを参照する。
// TODO: 名前、skelton のほうがいいかも
class MeshArmature
	: public Object
{
public:
	int boneCount() const { return m_bones.size(); }
	MeshBone* bone(int index) const;


	const List<MeshBone*>& rootBones() const { return m_rootBones; }

public:
	// TODO: internal
	void addBone(int linkNode, const Matrix& inverseInitialMatrix);

LN_CONSTRUCT_ACCESS:
	MeshArmature();
	bool init(SkinnedMeshModel* model);

public:	// TODO:
	SkinnedMeshModel* m_model = nullptr;
	List<Ref<MeshBone>> m_bones;
	List<MeshBone*> m_rootBones;

};

/**
 * SkinnedMeshModel
 */
LN_CLASS()
class SkinnedMeshModel
	: public StaticMeshModel
	//, public IAnimationTargetObject
{
public:
	/** load */
	LN_METHOD()
	static Ref<SkinnedMeshModel> load(const StringRef& filePath);

	/** animationController */
	LN_METHOD(Property)
	AnimationController* animationController() const;

	//void addMeshContainer(MeshContainer* meshContainer);
	//void addMaterial(Material* material);


	//const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
	//const List<Ref<Material>>& materials() const { return m_materials; }


	MeshNode* findHumanoidBone(HumanoidBones boneKind) const;

    // TODO: internal
	void addSkeleton(MeshArmature* skeleton);
	const Ref<MeshArmature>& skeleton(int index) const { return m_skeletons[index]; }
	const List<Ref<MeshArmature>>& skeletons() const { return m_skeletons; }
	void setWorldTransform(const Matrix& matrix) { m_worldTransform = matrix; m_worldTransformInverse = Matrix::makeInverse(m_worldTransform); }
    void beginUpdate();
	void preUpdate();
	void postUpdate();


	void updateBoneTransformHierarchy();
    void updateIK();

    void writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer);

    //List<Ref<SkinnedMeshBone>>		m_allBoneList;				// 全ボーンリスト
    //List<SkinnedMeshBone*>			m_ikBoneList;
    //List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
	
	List<Ref<MeshBoneIK>> m_iks;

	Ref<AnimationController> m_animationController;

	//static HumanoidBones mapToHumanoidBones(const MeshBone* bone);
	void verifyHumanoidBones();

	//float m_animationTranslationBasis = 1.0f;

//protected:
//	virtual int getAnimationTargetElementCount() const override;
//	virtual const String& getAnimationTargetElementName(int index) const override;
//	virtual AnimationValueType getAnimationTargetElementValueType(int index) const override;
//	virtual void setAnimationTargetElementValue(int index, const AnimationValue& value) override;

	std::array<int, 56> m_humanoidBoneNodeIndices;	// Index of m_bones
	void setHumanoidBoneIndex(HumanoidBones kind, int boneIndex) { m_humanoidBoneNodeIndices[static_cast<int>(kind)] = boneIndex; }
	int humanoidBoneIndex(HumanoidBones kind) const { return m_humanoidBoneNodeIndices[static_cast<int>(kind)]; }

	Ref<detail::MeshModelInstance> createMeshModelInstance();

LN_CONSTRUCT_ACCESS:
    SkinnedMeshModel();

private:
	List<Ref<MeshArmature>> m_skeletons;

	Matrix		m_worldTransform;
	Matrix		m_worldTransformInverse;
};

class MeshDiag
{
public:
	static void printNodes(const SkinnedMeshModel* model);
	static void clearBoneInitialRotations(SkinnedMeshModel* model);
};

} // namespace ln


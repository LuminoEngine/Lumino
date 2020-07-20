
#pragma once
#include "../Animation/AnimationMixer.hpp"
#include "Mesh.hpp"

namespace ln {
class SkinnedMeshModel;
class MeshArmature;
class Texture2D;
class AnimationController;

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

// 標準ボーン
enum class HumanoidBones
{
	None,

	// Body
	Hips,
	Spine,
	Chest,		// opt
	UpperChest,	// opt

	// Left Arm
	LeftShoulder,	// opt 鎖骨原点
	LeftUpperArm,	// 型原点
	LeftLowerArm,	// 肘原点
	LeftHand,		// 手首原点

	// Right Arm
	RightShoulder,	// opt 鎖骨原点
	RightUpperArm,	// 型原点
	RightLowerArm,	// 肘原点
	RightHand,		// 手首原点

	// Left Leg
	LeftUpperLeg,
	LeftLowerLeg,	// 膝原点
	LeftFoot,		// かかと原点
	LeftToes,		// opt つまさき

	// Right Leg
	RightUpperLeg,
	RightLowerLeg,	// 膝原点
	RightFoot,		// かかと原点
	RightToes,		// opt つまさき

	// Head
	Neck,			// opt 首下原点
	Head,			// 首上原点
	LeftEye,		// opt 
	RightEye,		// opt 
	Jaw,			// opt

	// Left Hand
	LeftThumbProximal,		// opt 親指 付け根
	LeftThumbIntermediate,	// opt 親指 中
	LeftThumbDistal,		// opt 親指 先
	LeftIndexProximal,		// opt 親指 付け根
	LeftIndexIntermediate,	// opt 親指 中
	LeftIndexDistal,		// opt 親指 先
	LeftMiddleProximal,		// opt 中指 付け根
	LeftMiddleIntermediate,	// opt 中指 中
	LeftMiddleDistal,		// opt 中指 先
	LeftRingProximal,		// opt 薬指 付け根
	LeftRingIntermediate,	// opt 薬指 中
	LeftRingDistal,			// opt 薬指 先
	LeftLittleProximal,		// opt 子指 付け根
	LeftLittleIntermediate,	// opt 子指 中
	LeftLittleDistal,		// opt 子指 先

	// Right Hand
	RightThumbProximal,		// opt 親指 付け根
	RightThumbIntermediate,	// opt 親指 中
	RightThumbDistal,		// opt 親指 先
	RightIndexProximal,		// opt 親指 付け根
	RightIndexIntermediate,	// opt 親指 中
	RightIndexDistal,		// opt 親指 先
	RightMiddleProximal,		// opt 中指 付け根
	RightMiddleIntermediate,	// opt 中指 中
	RightMiddleDistal,		// opt 中指 先
	RightRingProximal,		// opt 薬指 付け根
	RightRingIntermediate,	// opt 薬指 中
	RightRingDistal,			// opt 薬指 先
	RightLittleProximal,		// opt 子指 付け根
	RightLittleIntermediate,	// opt 子指 中
	RightLittleDistal,		// opt 子指 先
};

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

private:
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
	void updateSkinningMatrices(SkinnedMeshModel* model);
	const Ref<Texture2D>& skinningMatricesTexture() const { return m_skinningMatricesTexture; }

LN_CONSTRUCT_ACCESS:
	MeshArmature();
	bool init(SkinnedMeshModel* model);

public:	// TODO:
	SkinnedMeshModel* m_model = nullptr;
	List<Ref<MeshBone>> m_bones;
	List<MeshBone*> m_rootBones;
	Ref<Texture2D> m_skinningMatricesTexture;

};

class SkinnedMeshModel
	: public StaticMeshModel
	//, public IAnimationTargetObject
{
public:
	static Ref<SkinnedMeshModel> load(const StringRef& filePath);

	AnimationController* animationController() const;

	//void addMeshContainer(MeshContainer* meshContainer);
	//void addMaterial(Material* material);


	//const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
	//const List<Ref<Material>>& materials() const { return m_materials; }


    // TODO: internal
	void addSkeleton(MeshArmature* skeleton);
	const Ref<MeshArmature>& skeleton(int index) const { return m_skeletons[index]; }
	void setWorldTransform(const Matrix& matrix) { m_worldTransform = matrix; m_worldTransformInverse = Matrix::makeInverse(m_worldTransform); }
    void beginUpdate();
	void preUpdate();
	void postUpdate();


	void updateBoneTransformHierarchy();
    void updateIK();
	void updateSkinningMatrices();

    void writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer);

    //List<Ref<SkinnedMeshBone>>		m_allBoneList;				// 全ボーンリスト
    //List<SkinnedMeshBone*>			m_ikBoneList;
    //List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
	
	List<Ref<MeshBoneIK>> m_iks;

	Ref<AnimationController> m_animationController;

	//static HumanoidBones mapToHumanoidBones(const MeshBone* bone);

//protected:
//	virtual int getAnimationTargetElementCount() const override;
//	virtual const String& getAnimationTargetElementName(int index) const override;
//	virtual AnimationValueType getAnimationTargetElementValueType(int index) const override;
//	virtual void setAnimationTargetElementValue(int index, const AnimationValue& value) override;

	std::array<int, 56> m_humanoidBoneNodeIndices;	// Index of m_bones
	void setHumanoidBoneIndex(HumanoidBones kind, int boneIndex) { m_humanoidBoneNodeIndices[static_cast<int>(kind)] = boneIndex; }
	int humanoidBoneIndex(HumanoidBones kind) const { return m_humanoidBoneNodeIndices[static_cast<int>(kind)]; }


LN_CONSTRUCT_ACCESS:
    SkinnedMeshModel();

private:
	List<Ref<MeshArmature>> m_skeletons;

	Matrix		m_worldTransform;
	Matrix		m_worldTransformInverse;
};


/** スキンメッシュアニメーションにおいてキャラクターの挙動を操作するためのクラスです。 */
class AnimationController final
	: public Object
	, public detail::IAnimationMixerCoreHolder
{
public:

	/** アニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(AnimationClip* animationClip) { return m_core->addClip(animationClip); }

	/** ステート名を指定してアニメーションクリップを追加します。 (レイヤー0 へ追加されます) */
	AnimationState* addClip(const StringRef& stateName, AnimationClip* animationClip) { m_core->addClip(stateName, animationClip); }

	/** アニメーションクリップを除外します。 (レイヤー0 から除外されます) */
	void removeClip(AnimationClip* animationClip) { m_core->removeClip(animationClip); }

	/// 再生中であるかを確認する
	//bool isPlaying() const;

	/// 再生
	void play(const StringRef& stateName, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(stateName, duration); }
	void play(AnimationState* state, float duration = 0.3f/*, PlayMode mode = PlayMode_StopSameLayer*/) { m_core->play(state, duration); }

	///// ブレンド (アニメーションの再生には影響しない。停止中のアニメーションがこの関数によって再生開始されることはない)
	//void Blend(const lnKeyChar* animName, lnFloat targetWeight, lnFloat fadeLength);

	///// クロスフェード
	//void CrossFade(const lnKeyChar* animName, lnFloat fadeLength, PlayMode mode = StopSameLayer);

	///// 前のアニメーションが終了した後、再生を開始する
	//void PlayQueued(const lnKeyChar* animName, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer);

	///// 前のアニメーションが終了するとき、クロスフェードで再生を開始する
	//void CrossFadeQueued(const lnKeyChar* animName, lnFloat fadeLength, QueueMode queueMode = CompleteOthers, PlayMode playMode = StopSameLayer);

	///// 同レイヤー内のアニメーション再生速度の同期
	//void SyncLayer(int layer);


public:
	void advanceTime(float elapsedTime);


	///// AnimationTargetEntity の検索 (見つからなければ NULL)
	//detail::AnimationTargetAttributeEntity* findAnimationTargetAttributeEntity(const String& name);


LN_CONSTRUCT_ACCESS:
	AnimationController();
	bool init(SkinnedMeshModel* model);

protected:
	detail::AnimationTargetElementBlendLink* onRequireBinidng(const String& name) override;
	void onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding) override;

private:
	SkinnedMeshModel* m_model;
	Ref<AnimationMixerCore> m_core;
	List<Ref<detail::AnimationTargetElementBlendLink>> m_bindings;
};

} // namespace ln


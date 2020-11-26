
#pragma once
#include "../Animation/AnimationMixer.hpp"
#include "Common.hpp"
#include "StaticMeshModel.hpp"

namespace ln {
class SkinnedMeshModel;
class MeshSkeleton;
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


	//void addMeshContainer(MeshContainer* meshContainer);
	//void addMaterial(Material* material);


	//const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
	//const List<Ref<Material>>& materials() const { return m_materials; }


	MeshNode* findHumanoidBone(HumanoidBones boneKind) const;

    void beginUpdate();
	void preUpdate();
	void postUpdate();


	void updateBoneTransformHierarchy();
    void updateIK();

    void writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer);

    //List<Ref<SkinnedMeshBone>>		m_allBoneList;				// 全ボーンリスト
    //List<SkinnedMeshBone*>			m_ikBoneList;
    //List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
	


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
};

class MeshDiag
{
public:
	static void printNodes(const SkinnedMeshModel* model);
	static void clearBoneInitialRotations(SkinnedMeshModel* model);
};

} // namespace ln


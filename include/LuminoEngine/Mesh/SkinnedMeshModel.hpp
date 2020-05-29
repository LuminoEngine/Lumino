
#pragma once
#include "../Animation/AnimationController.hpp"
#include "Mesh.hpp"

namespace ln {
class SkinnedMeshModel;
class MeshArmature;
class Texture2D;

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
// 頂点の BLEND_INDICES から参照されるのはこのインスタンス。
// SkinnedMesh でのみ使用する。
class MeshBone
	: public Object
{
public:
	//const String& name() const;

private:
	int m_node = -1;

	// ボーンの初期姿勢を打ち消して、原点に戻す行列。
	// モデル空間内の絶対座標系となっている。(親ボーンからの相対座標系ではない点に注意)
	Matrix m_inverseInitialMatrix;

	friend class MeshArmature;
};

// Bone をまとめるデータ構造。
// BoneTexture を生成する単位。
// TODO: 名前、skelton のほうがいいかも
class MeshArmature
	: public Object
{
public:
	MeshBone* bone(int index) const;

public:
	// TODO: internal
	void addBone(int linkNode, const Matrix& inverseInitialMatrix);
	void updateSkinningMatrices(SkinnedMeshModel* model);
	const Ref<Texture2D>& skinningMatricesTexture() const { return m_skinningMatricesTexture; }

LN_CONSTRUCT_ACCESS:
	MeshArmature();
	bool init();

private:
	List<Ref<MeshBone>> m_bones;
	Ref<Texture2D> m_skinningMatricesTexture; 
};

class SkinnedMeshModel
	: public StaticMeshModel
	, public IAnimationTargetObject
{
public:
	static Ref<SkinnedMeshModel> load(const StringRef& filePath);

	AnimationController* animationController() const { return m_animationController; }

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
	Ref<AnimationController> m_animationController;

protected:
	virtual int getAnimationTargetElementCount() const override;
	virtual const String& getAnimationTargetElementName(int index) const override;
	virtual AnimationValueType getAnimationTargetElementValueType(int index) const override;
	virtual void setAnimationTargetElementValue(int index, const AnimationValue& value) override;

LN_CONSTRUCT_ACCESS:
    SkinnedMeshModel();

private:
	//List<Ref<MeshContainer>> m_meshContainers;
	//List<Ref<Material>> m_materials;
	List<Ref<MeshArmature>> m_skeletons;

	Matrix		m_worldTransform;
	Matrix		m_worldTransformInverse;
};

} // namespace ln


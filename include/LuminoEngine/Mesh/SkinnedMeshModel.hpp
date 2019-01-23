
#pragma once
#include "../Animation/AnimationController.hpp"
#include "Mesh.hpp"

namespace ln {
class SkinnedMeshModel;
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

class SkinnedMeshModel
	: public StaticMeshModel
	, public IAnimationTargetObject
{
public:
	AnimationController* animationController() const { return m_animationController; }

	//void addMeshContainer(MeshContainer* meshContainer);
	//void addMaterial(AbstractMaterial* material);


	//const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
	//const List<Ref<AbstractMaterial>>& materials() const { return m_materials; }


    // TODO: internal
	void setWorldTransform(const Matrix& matrix) { m_worldTransform = matrix; m_worldTransformInverse = Matrix::makeInverse(m_worldTransform); }
    void beginUpdate();
	void preUpdate();
	void postUpdate();


	void updateBoneTransformHierarchy();
    void updateIK();
	void updateSkinningMatrices();

    void writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer);

    List<Ref<SkinnedMeshBone>>		m_allBoneList;				// 全ボーンリスト
    List<SkinnedMeshBone*>			m_ikBoneList;
    List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)
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
	//List<Ref<AbstractMaterial>> m_materials;

	Matrix		m_worldTransform;
	Matrix		m_worldTransformInverse;
};

} // namespace ln


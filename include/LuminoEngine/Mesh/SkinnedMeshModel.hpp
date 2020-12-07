
#pragma once
#include "../Animation/AnimationMixer.hpp"
#include "Common.hpp"
#include "MeshModel.hpp"

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
	: public MeshModel
	//, public IAnimationTargetObject
{
public:

LN_CONSTRUCT_ACCESS:
    SkinnedMeshModel();

private:
};


} // namespace ln


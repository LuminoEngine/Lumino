#pragma once
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>

namespace ln {
class MeshModel;
namespace detail {
class MeshModelInstance;
class SkeletonInstance;

class BoneInstance
	: public RefObject
{
public:
	BoneInstance(SkeletonInstance* owner, int boneIndex);

	SkeletonInstance* owner() const { return m_owner; }
	MeshNode* node() const;
	const Matrix& combinedTransform() const;

private:
	SkeletonInstance* m_owner;
	int m_boneIndex;
};

class SkeletonInstance
	: public RefObject
{
public:
	SkeletonInstance(MeshModelInstance* owner, int skeletonIndex);

	MeshModelInstance* owner() const { return m_owner; }
	const Ref<MeshSkeleton>& skeletonModel() const;
	const List<Ref<BoneInstance>>& bones() const { return m_bones; }
	const Ref<Texture2D>& skinningMatricesTexture() const { return m_skinningMatricesTexture; }

	void updateSkinningMatrices();

private:
	MeshModelInstance* m_owner;
	int m_skeletonIndex;
	List<Ref<BoneInstance>> m_bones;
	Ref<Texture2D> m_skinningMatricesTexture;
};

class MorphInstance
	: public RefObject
{
public:
	MorphInstance(MeshModelInstance* owner, int meshContainerIndex);
	int meshContainerIndex() const { return m_meshContainerIndex; }
	void setWeight(int index, float value);
	float weight(int index) const { return m_weights[index]; }
	const std::array<int, MaxRenderMorphTargets> priorityTargets() const { return m_priorityTargets; }

	void updatePriorityTargets();
	bool getMorphWeights(std::array<float, MaxRenderMorphTargets>* outMorphWeights) const;

	Ref<VertexBuffer> m_blendResult;

private:
	MeshModelInstance* m_owner;
	int m_meshContainerIndex;
	std::vector<float> m_weights;
	std::array<int, MaxRenderMorphTargets> m_priorityTargets;
};

// MeshModel の描画にかかわる動的なデータ
class MeshModelInstance
	: public RefObject
{
public:


	MeshModelInstance(MeshModel* model);

	const Ref<MeshModel>& model() const { return m_model; }

	bool hasSkeleton() const { return !m_skeletons.isEmpty(); }
	bool hasMorph() const { return !m_morphs.isEmpty(); }

	const List<Ref<SkeletonInstance>>& skeletons() const { return m_skeletons; }
	const List<Ref<MorphInstance>>& morphs() const { return m_morphs; }

	void updateSkinningMatrices();

private:
	Ref<MeshModel> m_model;
	List<Ref<SkeletonInstance>> m_skeletons;
	List<Ref<MorphInstance>> m_morphs;
};

} // namespace detail
} // namespace ln

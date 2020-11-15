#pragma once


namespace ln {
class SkinnedMeshModel;
namespace detail {
class MeshModelInstance;

class SkeletonInstance
	: public RefObject
{
public:
	SkeletonInstance(MeshModelInstance* owner, int skeletonIndex);
	void updateSkinningMatrices();

	const Ref<Texture2D>& skinningMatricesTexture() const { return m_skinningMatricesTexture; }

private:
	MeshModelInstance* m_owner;
	int m_skeletonIndex;
	Ref<Texture2D> m_skinningMatricesTexture;
};

// MeshModel の描画にかかわる動的なデータ
class MeshModelInstance
	: public RefObject
{
public:
	struct BoneInstance
	{

	};


	MeshModelInstance(SkinnedMeshModel* model);

	const Ref<SkinnedMeshModel>& model() const { return m_model; }
	const List<Ref<SkeletonInstance>>& skeletons() const { return m_skeletons; }

	void updateSkinningMatrices();

private:
	Ref<SkinnedMeshModel> m_model;
	List<Ref<SkeletonInstance>> m_skeletons;
};

} // namespace detail
} // namespace ln

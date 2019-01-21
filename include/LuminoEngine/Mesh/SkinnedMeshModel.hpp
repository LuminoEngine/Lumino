
#pragma once
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
    void addChildBone(SkinnedMeshBone* bone);
    void postInitialize(SkinnedMeshModel* owner, int depth);

    // TODO: internal
    Ref<detail::PmxBoneResource> m_data;
    int m_boneIndex = 0;
    int m_depth = 0;

    SkinnedMeshBone*		m_parent = nullptr;
    List<SkinnedMeshBone*>	m_children;			// 子ボーンリスト
    Ref<detail::PmxIKResource>			m_ikInfo;
};

class SkinnedMeshModel
	: public StaticMeshModel
{
public:
	//void addMeshContainer(MeshContainer* meshContainer);
	//void addMaterial(AbstractMaterial* material);


	//const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
	//const List<Ref<AbstractMaterial>>& materials() const { return m_materials; }

    // TODO: internal
    List<Ref<SkinnedMeshBone>>		m_allBoneList;				// 全ボーンリスト
    List<SkinnedMeshBone*>			m_ikBoneList;
    List<SkinnedMeshBone*>			m_rootBoneList;				// ルートボーンリスト (親を持たないボーンリスト)

private:
	//List<Ref<MeshContainer>> m_meshContainers;
	//List<Ref<AbstractMaterial>> m_materials;
};

} // namespace ln


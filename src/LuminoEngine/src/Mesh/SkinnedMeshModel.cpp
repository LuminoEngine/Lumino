
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "MeshManager.hpp"

namespace ln {


//==============================================================================
// SkinnedMeshBone

void SkinnedMeshBone::postInitialize(SkinnedMeshModel* owner, int depth)
{
    m_depth = depth;

    for (SkinnedMeshBone* bone : m_children)
    {
        bone->postInitialize(owner, m_depth + 1);
    }
}

void SkinnedMeshBone::addChildBone(SkinnedMeshBone* bone)
{
    if (LN_REQUIRE(bone != nullptr)) return;
    if (LN_REQUIRE(bone->m_parent == nullptr)) return;
    m_children.add(bone);
    bone->m_parent = this;
}

//==============================================================================
// SkinnedMeshModel

//void SkinnedMeshModel::addMeshContainer(MeshContainer* meshContainer)
//{
//	if (LN_REQUIRE(meshContainer)) return;
//	if (LN_REQUIRE(!meshContainer->m_meshModel)) return;
//	m_meshContainers.add(meshContainer);
//	meshContainer->m_meshModel = this;
//}
//
//void SkinnedMeshModel::addMaterial(AbstractMaterial* material)
//{
//	m_materials.add(material);
//}

} // namespace ln


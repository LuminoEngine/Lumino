
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

SkinnedMeshModel::SkinnedMeshModel()
    : StaticMeshModel(detail::InternalMeshModelType::SkinnedMesh)
{
}

void SkinnedMeshModel::writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer)
{
    for (int i = 0; i < m_allBoneList.size(); i++)
    {
        matrixesBuffer[i] = Matrix::Identity;
        localQuaternionsBuffer[i] = Quaternion::Identity;
    }
}


} // namespace ln


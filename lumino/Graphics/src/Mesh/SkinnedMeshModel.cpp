#include "Internal.hpp"
#include <LuminoGraphics/Bitmap/Bitmap.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/AnimationController.hpp>
#include <LuminoGraphics/Mesh/SkinnedMeshModel.hpp>
#include <LuminoGraphics/Mesh/MeshModeEntity.hpp>
#include "MeshImporters/PmxImporter.hpp"	// TODO: 依存したくない
#include "MeshManager.hpp"
#include "CCDIKSolver.hpp"

namespace ln {

#if 0
//==============================================================================
// SkinnedMeshBone

const String& SkinnedMeshBone::name() const
{
	return m_data->Name;
}

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

void SkinnedMeshBone::updateGlobalTransform(bool hierarchical)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	//m_combinedMatrix = m_localTransform;
	// TODO: * ではなく一気に作ったほうがはやいかも
    m_combinedMatrix =
        //Matrix::makeTranslation(-m_data->OrgPosition) *
        Matrix::makeScaling(m_localTransform.scale) *
        Matrix::makeRotationQuaternion(m_localTransform.rotation) *
        Matrix::makeTranslation(m_localTransform.translation);// *
        //Matrix::makeTranslation(m_data->OrgPosition);


															 // 親からの平行移動量
	m_combinedMatrix.translate(m_data->getOffsetFromParent());

	//m_combinedMatrix =
	//	Matrix::MakeTranslation(-m_core->OrgPosition) *
	//	Matrix::makeRotationQuaternion(m_localTransform.rotation) *
	//	Matrix::MakeTranslation(m_localTransform.translation) *
	//	Matrix::MakeTranslation(m_core->OrgPosition);

	// 親行列と結合
	if (m_parent != nullptr)
	{
		m_combinedMatrix *= m_parent->getCombinedMatrix();
	}

	// 子ボーン更新
	if (hierarchical)
	{
		for (SkinnedMeshBone* bone : m_children)
		{
			bone->updateGlobalTransform(hierarchical);
		}
	}
}
#endif

//==============================================================================
// SkinnedMeshModel

//Ref<SkinnedMeshModel> SkinnedMeshModel::load(const StringView& filePath)
//{
//	return detail::EngineDomain::meshManager()->createSkinnedMeshModel(filePath, 1.0f);
//}

SkinnedMeshModel::SkinnedMeshModel()
    : MeshModel(detail::InternalMeshModelType::SkinnedMesh)
{

	std::fill(m_humanoidBoneNodeIndices.begin(), m_humanoidBoneNodeIndices.end(), -1);
}

} // namespace ln

